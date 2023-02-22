/*
* Percepio Trace Recorder for Tracealyzer v4.7.0
* Copyright 2023 Percepio AB
* www.percepio.com
*
* SPDX-License-Identifier: Apache-2.0
*/

/**
 * @file 
 * 
 * @brief Public trace entry table APIs.
 */

#ifndef TRC_ENTRY_TABLE_H
#define TRC_ENTRY_TABLE_H

#if (TRC_USE_TRACEALYZER_RECORDER == 1)

#if (TRC_CFG_RECORDER_MODE == TRC_RECORDER_MODE_STREAMING)

#include <trcTypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup trace_entry_table_apis Trace Entry Table APIs
 * @ingroup trace_recorder_apis
 * @{
 */

#define TRC_ENTRY_CREATE_WITH_ADDRESS(_pvAddress, _pxEntryHandle) (xTraceEntryCreate(_pxEntryHandle) == TRC_SUCCESS ? (((TraceEntry_t*)*(_pxEntryHandle))->pvAddress = (_pvAddress), TRC_SUCCESS) : TRC_FAIL)
#define TRC_ENTRY_SET_STATE(xEntryHandle, uiStateIndex, uxState) TRC_COMMA_EXPR_TO_STATEMENT_EXPR_2(((TraceEntry_t*)(xEntryHandle))->xStates[uiStateIndex] = (uxState), TRC_SUCCESS)
#define TRC_ENTRY_SET_OPTIONS(xEntryHandle, uiMask) TRC_COMMA_EXPR_TO_STATEMENT_EXPR_2(((TraceEntry_t*)(xEntryHandle))->uiOptions |= (uiMask), TRC_SUCCESS)
#define TRC_ENTRY_CLEAR_OPTIONS(xEntryHandle, uiMask) TRC_COMMA_EXPR_TO_STATEMENT_EXPR_2(((TraceEntry_t*)(xEntryHandle))->uiOptions &= ~(uiMask), TRC_SUCCESS)
#define TRC_ENTRY_GET_ADDRESS(xEntryHandle, ppvAddress) TRC_COMMA_EXPR_TO_STATEMENT_EXPR_2(*(ppvAddress) = ((TraceEntry_t*)(xEntryHandle))->pvAddress, TRC_SUCCESS)
#define TRC_ENTRY_GET_SYMBOL(xEntryHandle, pszSymbol) TRC_COMMA_EXPR_TO_STATEMENT_EXPR_2(*(pszSymbol) = ((TraceEntry_t*)(xEntryHandle))->szSymbol, TRC_SUCCESS)
#define TRC_ENTRY_GET_STATE(xEntryHandle, uiStateIndex, puxState) TRC_COMMA_EXPR_TO_STATEMENT_EXPR_2(*(puxState) = ((TraceEntry_t*)(xEntryHandle))->xStates[uiStateIndex], TRC_SUCCESS)
#define TRC_ENTRY_GET_STATE_RETURN(xEntryHandle, uiStateIndex) (((TraceEntry_t*)(xEntryHandle))->xStates[uiStateIndex])
#define TRC_ENTRY_GET_OPTIONS(xEntryHandle, puiOptions) TRC_COMMA_EXPR_TO_STATEMENT_EXPR_2(*(puiOptions) = ((TraceEntry_t*)(xEntryHandle))->uiOptions, TRC_SUCCESS)

#define TRC_ENTRY_TABLE_SLOTS (TRC_CFG_ENTRY_SLOTS)
#define TRC_ENTRY_TABLE_STATE_COUNT (3UL)
#define TRC_ENTRY_TABLE_SYMBOL_LENGTH  ((uint32_t)(TRC_CFG_ENTRY_SYMBOL_MAX_LENGTH))
#define TRC_ENTRY_TABLE_SLOT_SYMBOL_SIZE ((((sizeof(char) * (uint32_t)(TRC_ENTRY_TABLE_SYMBOL_LENGTH)) + (sizeof(uint32_t) - 1UL)) / sizeof(uint32_t)) * sizeof(uint32_t))

#if (TRC_ENTRY_TABLE_SLOTS > 256UL)
typedef uint16_t TraceEntryIndex_t;
#else
typedef uint8_t TraceEntryIndex_t;
#endif

typedef struct EntryIndexTable
{
	TraceEntryIndex_t axFreeIndexes[TRC_ENTRY_TABLE_SLOTS];
	uint32_t uiFreeIndexCount;
} TraceEntryIndexTable_t;

/** Trace Entry Structure */
typedef struct TraceEntry
{
	void* pvAddress;												/**< */
	TraceUnsignedBaseType_t xStates[TRC_ENTRY_TABLE_STATE_COUNT];	/**< */
	uint32_t uiOptions;												/**< */
	char szSymbol[TRC_ENTRY_TABLE_SLOT_SYMBOL_SIZE];				/**< */
} TraceEntry_t;

typedef struct TraceEntryTable
{
	TraceUnsignedBaseType_t uxSlots;
	TraceUnsignedBaseType_t uxEntrySymbolLength;
	TraceUnsignedBaseType_t uxEntryStateCount;
	TraceEntry_t axEntries[TRC_ENTRY_TABLE_SLOTS];
} TraceEntryTable_t;

/**
 * @internal Initialize trace entry index table.
 * 
 * This routine initializes the trace entry index table which keeps track
 * of availables indexes.
 * 
 * @param[in] pxBuffer Pointer to uninitialized trace entry index table buffer.
 * 
 * @retval TRC_FAIL Failure
 * @retval TRC_SUCCESS Success
 */
traceResult xTraceEntryIndexTableInitialize(TraceEntryIndexTable_t* const pxBuffer);

/**
 * @internal Initialize trace entry table.
 * 
 * This routine initializes the trace entry table which maps objects to
 * symbolic identifiers, state information, and options.
 * 
 * @param[in] pxBuffer Pointer to uninitialized trace entry table buffer.
 * 
 * @retval TRC_FAIL Failure
 * @retval TRC_SUCCESS Success
 */
traceResult xTraceEntryTableInitialize(TraceEntryTable_t* const pxBuffer);

/**
 * @brief Creates trace entry.
 * 
 * @param[out] pxEntryHandle Pointer to uninitialized trace entry handle.
 * 
 * @retval TRC_FAIL Failure
 * @retval TRC_SUCCESS Success
 */
traceResult xTraceEntryCreate(TraceEntryHandle_t *pxEntryHandle);

/**
 * @brief Deletes trace entry.
 * 
 * @param[in] xEntryHandle Pointer to initialized trace entry handle.
 * 
 * @retval TRC_FAIL Failure
 * @retval TRC_SUCCESS Success
 */
traceResult xTraceEntryDelete(TraceEntryHandle_t xEntryHandle);

/**
 * @brief Finds trace entry mapped to object address.
 * 
 * @param[in] pvAddress Address of object.
 * @param[out] pxEntryHandle Pointer to uninitialized trace entry handle.
 * 
 * @retval TRC_FAIL Failure
 * @retval TRC_SUCCESS Success
 */
traceResult xTraceEntryFind(const void* const pvAddress, TraceEntryHandle_t* pxEntryHandle);

/**
 * @brief Gets the number of entries in the trace entry table.
 * 
 * @param[out] puiCount Count.
 * 
 * @retval TRC_FAIL Failure
 * @retval TRC_SUCCESS Success
 */
traceResult xTraceEntryGetCount(uint32_t* puiCount);

/**
 * @brief Gets trace table entry at index. 
 * 
 * @param[in] index Entry index.
 * @param[out] pxEntryHandle Pointer to uninitialized trace entry handle. 
 * 
 * @retval TRC_FAIL Failure
 * @retval TRC_SUCCESS Success
 */
traceResult xTraceEntryGetAtIndex(uint32_t index, TraceEntryHandle_t* pxEntryHandle);

/**
 * @brief Sets symbol for entry.
 * 
 * @param[in] xEntryHandle Pointer to initialized trace entry handle.
 * @param[in] szSymbol Pointer to symbol string, set by function
 * @param[in] uiLength Symbol length
 * 
 * @retval TRC_FAIL Failure
 * @retval TRC_SUCCESS Success
 */
traceResult xTraceEntrySetSymbol(const TraceEntryHandle_t xEntryHandle, const char* szSymbol, uint32_t uiLength);

#if ((TRC_CFG_USE_TRACE_ASSERT) == 1)

/**
 * @brief Creates trace entry mapped to memory address.
 * 
 * @param[in] pvAddress Address.
 * @param[out] pxEntryHandle Pointer to uninitialized trace entry handle.
 * 
 * @retval TRC_FAIL Failure
 * @retval TRC_SUCCESS Success
 */
traceResult xTraceEntryCreateWithAddress(void* const pvAddress, TraceEntryHandle_t* pxEntryHandle);

/**
 * @brief Sets trace entry state.
 * 
 * @param[in] xEntryHandle Pointer to initialized trace entry handle.
 * @param[in] uiStateIndex Index of state (< TRC_ENTRY_TABLE_STATE_COUNT).
 * @param[in] uxState State.
 * 
 * @retval TRC_FAIL Failure
 * @retval TRC_SUCCESS Success
 */
traceResult xTraceEntrySetState(const TraceEntryHandle_t xEntryHandle, uint32_t uiStateIndex, TraceUnsignedBaseType_t uxState);

/**
 * @brief Sets trace entry option(s).
 * 
 * @param[in] xEntryHandle Pointer to initialized trace entry handle.
 * @param[in] uiMask Option(s) set mask.
 * 
 * @retval TRC_FAIL Failure
 * @retval TRC_SUCCESS Success
 */
traceResult xTraceEntrySetOptions(const TraceEntryHandle_t xEntryHandle, uint32_t uiMask);

/**
 * @brief Clears trace entry option(s).
 * 
 * @param[in] xEntryHandle Pointer to initialized trace entry handle.
 * @param[in] uiMask Options(s) clear mask.
 * 
 * @retval TRC_FAIL Failure
 * @retval TRC_SUCCESS Success
 */
traceResult xTraceEntryClearOptions(const TraceEntryHandle_t xEntryHandle, uint32_t uiMask);

/**
 * @brief Gets linked address for trace entry.
 * 
 * @param[in] xEntryHandle Pointer to initialized trace entry handle.
 * @param[out] ppvAddress Address.
 * 
 * @retval TRC_FAIL Failure
 * @retval TRC_SUCCESS Success
 */
traceResult xTraceEntryGetAddress(const TraceEntryHandle_t xEntryHandle, void **ppvAddress);

/**
 * @brief Gets symbol for trace entry.
 * 
 * @param[in] xEntryHandle Pointer to initialized trace entry handle.
 * @param[out] pszSymbol Symbol.
 * 
 * @retval TRC_FAIL Failure
 * @retval TRC_SUCCESS Success
 */
traceResult xTraceEntryGetSymbol(const TraceEntryHandle_t xEntryHandle, const char** pszSymbol);

/**
 * @brief Gets state for trace entry.
 * 
 * @param[in] xEntryHandle Pointer to initialized trace entry handle.
 * @param[in] uiStateIndex State index (< TRC_ENTRY_TABLE_STATE_COUNT).
 * @param[out] puxState State.
 * 
 * @retval TRC_FAIL Failure
 * @retval TRC_SUCCESS Success
 */
traceResult xTraceEntryGetState(const TraceEntryHandle_t xEntryHandle, uint32_t uiStateIndex, TraceUnsignedBaseType_t *puxState);

/**
 * @internal Returns state for trace entry.
 *
 * @param[in] xEntryHandle Pointer to initialized trace entry handle.
 * @param[in] uiStateIndex State index (< TRC_ENTRY_TABLE_STATE_COUNT).
 *
 * @returns State
 */
TraceUnsignedBaseType_t xTraceEntryGetStateReturn(const TraceEntryHandle_t xEntryHandle, uint32_t uiStateIndex);

/**
 * @brief Gets options for trace entry.
 * 
 * @param[in] xEntryHandle Pointer to initialized trace entry handle.
 * @param[out] puiOptions Options.
 * 
 * @retval TRC_FAIL Failure
 * @retval TRC_SUCCESS Success
 */
traceResult xTraceEntryGetOptions(const TraceEntryHandle_t xEntryHandle, uint32_t *puiOptions);

#else

#define xTraceEntryCreateWithAddress TRC_ENTRY_CREATE_WITH_ADDRESS

#define xTraceEntrySetState TRC_ENTRY_SET_STATE
#define xTraceEntrySetOptions TRC_ENTRY_SET_OPTIONS
#define xTraceEntryClearOptions TRC_ENTRY_CLEAR_OPTIONS

#define xTraceEntryGetAddress TRC_ENTRY_GET_ADDRESS
#define xTraceEntryGetSymbol TRC_ENTRY_GET_SYMBOL
#define xTraceEntryGetState TRC_ENTRY_GET_STATE
#define xTraceEntryGetStateReturn TRC_ENTRY_GET_STATE_RETURN
#define xTraceEntryGetOptions TRC_ENTRY_GET_OPTIONS

#endif /* ((TRC_CFG_USE_TRACE_ASSERT) == 1) */

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* (TRC_CFG_RECORDER_MODE == TRC_RECORDER_MODE_STREAMING) */

#endif /* (TRC_USE_TRACEALYZER_RECORDER == 1) */

#endif /* TRC_ENTRY_TABLE_H */