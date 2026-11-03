/*
 * A352734  Records of the total number of parts in all partitions of a
 *          positive integer into consecutive parts.
 * https://oeis.org/A352734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th record high of A204217(k), where A204217(k) =
 *            sum of the lengths of all partitions of k into consecutive
 *            parts. The value A204217(k) is computed as the sum over all
 *            divisors l of 2k satisfying s > l, (s – l + 1) even, and a = (s
 *            – l + 1)/2 ≥ 1, where s = 2k/l
 * Range:     u8 n <= 24, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Compute A204217(k): total number of parts in all partitions of k into consecutive parts. */
static seq_t
compute_A204217(idx_t k)
{
    idx_t two_k = k * 2;
    seq_t sum = 0;

    for (idx_t d = 1; d * d <= two_k; ++d) {
        if (two_k % d != 0) continue;

        /* first divisor l = d */
        {
            idx_t l  = d;
            idx_t s  = two_k / l;
            if (s > l) {
                idx_t a_num = s - l + 1;
                if ((a_num & 1u) == 0) {
                    idx_t a_val = a_num >> 1;
                    if (a_val >= 1) sum += (seq_t)l;
                }
            }
        }

        /* second divisor l = two_k / d, if distinct */
        idx_t other = two_k / d;
        if (other != d) {
            idx_t l  = other;
            idx_t s  = d;  /* since s = two_k / l */
            if (s > l) {
                idx_t a_num = s - l + 1;
                if ((a_num & 1u) == 0) {
                    idx_t a_val = a_num >> 1;
                    if (a_val >= 1) sum += (seq_t)l;
                }
            }
        }
    }

    return sum;
}

/* static state for memoised records */
static seq_t *records   = NULL;  /* record values a(1), a(2), ...   */
static size_t record_cap= 0;      /* allocated capacity of records    */
static size_t record_cnt= 0;      /* number of records computed so far */
static seq_t  max_val   = 0;      /* maximum A204217 value seen so far */
static idx_t  next_k    = 1;      /* next k to evaluate (start at 1)   */

/* Helper to ensure capacity of the records array */
static void
ensure_capacity(size_t needed)
{
    if (record_cap >= needed) return;
    record_cap = (record_cap == 0) ? 1 : record_cap * 2;
    while (record_cap < needed) record_cap *= 2;
    records = mem_resize(seq_t, records, record_cap);
}

/* Public function: returns the n-th record value of A204217(k). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0; return 0 safely */

    /* first time: ensure records array is ready */
    if (record_cnt == 0) {
        ensure_capacity(1);
    }

    /* compute further values until we have at least n records */
    while (record_cnt < n) {
        seq_t val = compute_A204217(next_k);
        if (val > max_val) {
            max_val = val;
            ensure_capacity(record_cnt + 1);
            records[record_cnt] = val;
            ++record_cnt;
        }
        ++next_k;
    }

    return records[n - 1];
}
