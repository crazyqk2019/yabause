
#include <core.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h> 
#include "sh2core.h"
#include "DynarecSh2.h"


#define SH2CORE_DYNAMIC             3

extern "C" {
int SH2DynInit(void);
void SH2DynDeInit(void);
void SH2DynReset(SH2_struct *context);
void FASTCALL SH2DynExec(SH2_struct *context, u32 cycles);
void SH2DynGetRegisters(SH2_struct *context, sh2regs_struct *regs);
u32 SH2DynGetGPR(SH2_struct *context, int num);
u32 SH2DynGetSR(SH2_struct *context);
u32 SH2DynGetGBR(SH2_struct *context);
u32 SH2DynGetVBR(SH2_struct *context);
u32 SH2DynGetMACH(SH2_struct *context);
u32 SH2DynGetMACL(SH2_struct *context);
u32 SH2DynGetPR(SH2_struct *context);
u32 SH2DynGetPC(SH2_struct *context);
void SH2DynSetRegisters(SH2_struct *context, const sh2regs_struct *regs);
void SH2DynSetGPR(SH2_struct *context, int num, u32 value);
void SH2DynSetSR(SH2_struct *context, u32 value);
void SH2DynSetGBR(SH2_struct *context, u32 value);
void SH2DynSetVBR(SH2_struct *context, u32 value);
void SH2DynSetMACH(SH2_struct *context, u32 value);
void SH2DynSetMACL(SH2_struct *context, u32 value);
void SH2DynSetPR(SH2_struct *context, u32 value);
void SH2DynSetPC(SH2_struct *context, u32 value);
void SH2DynSendInterrupt(SH2_struct *context, u8 level, u8 vector);
int SH2DynGetInterrupts(SH2_struct *context, interrupt_struct interrupts[MAX_INTERRUPTS]);
void SH2DynSetInterrupts(SH2_struct *context, int num_interrupts, const interrupt_struct interrupts[MAX_INTERRUPTS]);
void SH2DynWriteNotify(u32 start, u32 length);

SH2Interface_struct SH2Dyn = {
  SH2CORE_DYNAMIC,
  "SH2 Dynamic Recompiler",

  SH2DynInit,
  SH2DynDeInit,
  SH2DynReset,
  SH2DynExec,

  SH2DynGetRegisters,
  SH2DynGetGPR,
  SH2DynGetSR,
  SH2DynGetGBR,
  SH2DynGetVBR,
  SH2DynGetMACH,
  SH2DynGetMACL,
  SH2DynGetPR,
  SH2DynGetPC,

  SH2DynSetRegisters,
  SH2DynSetGPR,
  SH2DynSetSR,
  SH2DynSetGBR,
  SH2DynSetVBR,
  SH2DynSetMACH,
  SH2DynSetMACL,
  SH2DynSetPR,
  SH2DynSetPC,

  SH2DynSendInterrupt,
  SH2DynGetInterrupts,
  SH2DynSetInterrupts,
  SH2DynWriteNotify
};


Block * dCode;

int SH2DynInit(void) {
  return 0;
}

void SH2DynDeInit(void){
}

void SH2DynReset(SH2_struct *context) {

  if (context->ext == NULL){
    DynarecSh2 * pctx = new DynarecSh2();
    context->ext = (void*)pctx;
  }
  
  DynarecSh2 * pctx = (DynarecSh2*)context->ext;
  pctx->ResetCPU();
}

void FASTCALL SH2DynExec(SH2_struct *context, u32 cycles){
  DynarecSh2* pctx = ((DynarecSh2*)context->ext);
  pctx->SetCurrentContext();
  pctx->ExecuteCount(cycles);
}

void SH2DynSendInterrupt(SH2_struct *context, u8 vector, u8 level){
  DynarecSh2 *pctx = (DynarecSh2*)context->ext;
  pctx->AddInterrupt(vector, level);
}

int SH2DynGetInterrupts(SH2_struct *context, interrupt_struct interrupts[MAX_INTERRUPTS]){
  return 0;
}

void SH2DynSetInterrupts(SH2_struct *context, int num_interrupts, const interrupt_struct interrupts[MAX_INTERRUPTS]){
  return;
}

void SH2DynGetRegisters(SH2_struct *context, sh2regs_struct *regs) {
  DynarecSh2 *pctx = (DynarecSh2*)context->ext;
  memcpy(regs->R, pctx->GetGenRegPtr(), sizeof(regs->R));
  regs->GBR = pctx->GET_GBR();
  regs->VBR = pctx->GET_VBR();
  regs->SR.all = pctx->GET_SR();
  regs->MACH = pctx->GET_MACH();
  regs->MACL = pctx->GET_MACL();
  regs->PC = pctx->GET_PC();
  regs->PR = pctx->GET_PR();
}

u32 SH2DynGetGPR(SH2_struct *context, int num){
  DynarecSh2 *pctx = (DynarecSh2*)context->ext;
  return pctx->GetGenRegPtr()[num];
}

u32 SH2DynGetSR(SH2_struct *context){
  DynarecSh2 *pctx = (DynarecSh2*)context->ext;
  return pctx->GET_SR();
}

u32 SH2DynGetGBR(SH2_struct *context){
  DynarecSh2 *pctx = (DynarecSh2*)context->ext;
  return pctx->GET_GBR();
}

u32 SH2DynGetVBR(SH2_struct *context){
  DynarecSh2 *pctx = (DynarecSh2*)context->ext;
  return pctx->GET_VBR();
}

u32 SH2DynGetMACH(SH2_struct *context){
  DynarecSh2 *pctx = (DynarecSh2*)context->ext;
  return pctx->GET_MACH();
}

u32 SH2DynGetMACL(SH2_struct *context){
  DynarecSh2 *pctx = (DynarecSh2*)context->ext;
  return pctx->GET_MACL();
}

u32 SH2DynGetPR(SH2_struct *context){
  DynarecSh2 *pctx = (DynarecSh2*)context->ext;
  return pctx->GET_PR();
}

u32 SH2DynGetPC(SH2_struct *context){
  DynarecSh2 *pctx = (DynarecSh2*)context->ext;
  return pctx->GET_PC();
}

void SH2DynSetRegisters(SH2_struct *context, const sh2regs_struct *regs){
  DynarecSh2 *pctx = (DynarecSh2*)context->ext;
  memcpy(pctx->GetGenRegPtr(), regs->R , sizeof(regs->R));
  pctx->SET_MACH(regs->GBR);
  pctx->SET_VBR(regs->VBR);
  pctx->SET_SR(regs->SR.all);
  pctx->SET_MACH(regs->MACH);
  pctx->SET_MACL(regs->MACL);
  pctx->SET_PC(regs->PC);
  pctx->SET_PR(regs->PR);
}

void SH2DynSetGPR(SH2_struct *context, int num, u32 value) {
  DynarecSh2 *pctx = (DynarecSh2*)context->ext;
  pctx->GetGenRegPtr()[num] = value;
}

void SH2DynSetSR(SH2_struct *context, u32 value){
  DynarecSh2 *pctx = (DynarecSh2*)context->ext;
  pctx->SET_SR(value);
}

void SH2DynSetGBR(SH2_struct *context, u32 value){
  DynarecSh2 *pctx = (DynarecSh2*)context->ext;
  pctx->SET_GBR(value);
}

void SH2DynSetVBR(SH2_struct *context, u32 value){
  DynarecSh2 *pctx = (DynarecSh2*)context->ext;
  pctx->SET_VBR(value);
}

void SH2DynSetMACH(SH2_struct *context, u32 value){
  DynarecSh2 *pctx = (DynarecSh2*)context->ext;
  pctx->SET_MACH(value);
}

void SH2DynSetMACL(SH2_struct *context, u32 value){
  DynarecSh2 *pctx = (DynarecSh2*)context->ext;
  pctx->SET_MACL(value);
}
void SH2DynSetPR(SH2_struct *context, u32 value){
  DynarecSh2 *pctx = (DynarecSh2*)context->ext;
  pctx->SET_PR(value);
}
void SH2DynSetPC(SH2_struct *context, u32 value){
  DynarecSh2 *pctx = (DynarecSh2*)context->ext;
  pctx->SET_PC(value);
}

void SH2DynWriteNotify(u32 start, u32 length){
  CompileBlocks * block = CompileBlocks::getInstance();
  
  switch (start & 0x0FF00000){
    // ROM
  case 0x00000000:
      block->LookupTableRom[start & 0x000FFFFF] = NULL;
    break;

  // Low Memory
  case 0x00200000:
    block->LookupTableLow[start & 0x000FFFFF] = NULL;
    break;
    // High Memory
  case 0x06000000:
    block->LookupTable[(start & 0x000FFFFF) >> 1] = NULL;
    break;

    // Cache
  default:
    if ((start & 0xFF000000) == 0xC0000000){
      block->LookupTableC[start & 0x000FFFFF] = NULL;
    }
    break;
  }
}

void SH2DynShowSttaics(SH2_struct * master, SH2_struct * slave ){
  CompileBlocks * block = CompileBlocks::getInstance();

  DynarecSh2 *pctx = (DynarecSh2*)master->ext;
  pctx->ShowStatics();

  DynarecSh2 *pctxs = (DynarecSh2*)slave->ext;
  pctxs->ShowStatics();

  block->ShowStatics();
}

//********************************************************************
// MemoyAcess from DynarecCPU
//********************************************************************
void memSetByte(u32 addr , u8 data )
{
  dynaLock();
  CompileBlocks * block = CompileBlocks::getInstance();
  switch (addr & 0x0FF00000)
  {
  // Low Memory
  case 0x00200000:
    block->LookupTableLow[addr & 0x000FFFFF] = NULL;
    break;
  // High Memory
  case 0x06000000:
    block->LookupTable[(addr & 0x000FFFFF) >> 1] = NULL;
    break;

  // Cache
  default:
    if ((addr & 0xFF000000) == 0xC0000000)
    {
      block->LookupTableC[addr & 0x000FFFFF] = NULL;
    }
  }
  MappedMemoryWriteByte(addr, data);
  dynaFree();
}

void memSetWord(u32 addr, u16 data )
{
  dynaLock();
  CompileBlocks * block = CompileBlocks::getInstance();
  switch (addr & 0x0FF00000)
  {
  // Low Memory
   case 0x00200000:
    block->LookupTableLow[addr & 0x000FFFFF] = NULL;
    break;
  // High Memory
  case 0x06000000:
    block->LookupTable[(addr & 0x000FFFFF) >> 1] = NULL;
    break;

  // Cache
  default:
    if ((addr & 0xFF000000) == 0xC0000000)
    {
      block->LookupTableC[addr & 0x000FFFFF] = NULL;
    }
  }
  MappedMemoryWriteWord(addr, data);
  dynaFree();
}

void memSetLong(u32 addr , u32 data )
{
  dynaLock();
  CompileBlocks * block = CompileBlocks::getInstance();
  switch (addr & 0x0FF00000)
  {
    // Low Memory
  case 0x00200000:
    block->LookupTableLow[addr & 0x000FFFFF] = NULL;
    block->LookupTableLow[(addr & 0x000FFFFF)+2] = NULL;
    break;
  // High Memory
  case 0x06000000:
    block->LookupTable[(addr & 0x000FFFFF) >> 1] = NULL;
    block->LookupTable[((addr & 0x000FFFFF) >> 1) + 1] = NULL;
    break;

  // Cache
  default:
    if ((addr & 0xFF000000) == 0xC0000000)
    {
      block->LookupTableC[addr & 0x000FFFFF] = NULL;
    }
  }
  MappedMemoryWriteLong(addr, data);
  dynaFree();
}

u8 memGetByte(u32 addr)
{
  dynaLock();
  u8 val;
  val = MappedMemoryReadByte(addr);
  dynaFree();
  return val;
}
 
u16 memGetWord(u32 addr)
{
  dynaLock();
  u16 val;
  val = MappedMemoryReadWord(addr);
  dynaFree();
  return val;
}

u32 memGetLong(u32 addr)
{
  dynaLock();
  u32 val;
  val = MappedMemoryReadLong(addr);
  dynaFree();
  return val;
}


//************************************************
// Callbacks from DynarecCPU
//************************************************
int DelayEachClock() {
  return 0;
}

int DebugDeleayClock() {
  return 0;
}

int g_step = 1;

int DebugEachClock() {
  dynaLock();
#ifdef DMPHISTORY
  CurrentSH2->pchistory_index++;
  CurrentSH2->pchistory[CurrentSH2->pchistory_index & 0xFF] = DynarecSh2::CurrentContext->m_pDynaSh2->SysReg[3];
  CurrentSH2->regshistory[CurrentSH2->pchistory_index & 0xFF] = NULL;
#endif
//  DynaCheckBreakPoint(g_CurrentContext->GET_PC());

  if (DynarecSh2::CurrentContext->CheckInterupt()) {
      dynaFree();
      return 1;
  }

#if 0
  if (g_step){
    DynarecSh2::CurrentContext->m_pDynaSh2->SysReg[3] += 2;
    dynaFree();
    return 1;
  }
#endif

  dynaFree();
  return 0;
}
  
int EachClock() {
  dynaLock();
  if (DynarecSh2::CurrentContext->CheckInterupt()) {
      dynaFree();
      return 1;
  }
  dynaFree();
  return 0;
}



}