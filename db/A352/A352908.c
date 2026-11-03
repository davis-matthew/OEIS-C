/*
 * A352908  Numbers k such that A232324(k) is prime.
 * https://oeis.org/A352908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 30, u32 n <= 2988, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9661 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */
#include "../utils/mem.h"
#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif


/* 
 * a(n) = k where the k-th triangular number T_k = k*(k+1)/2
 * modulo the sum of divisors σ(k) is a prime.
 * The function returns the n-th such k (offset 1).
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_seq.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static state to remember progress across calls */
    static seq_t *seq_arr = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;
    static idx_t current_k = 1;      /* next k to test */

    /* Generate terms until we have at least n */
    while (seq_len < n) {
        seq_t T = (seq_t)current_k * ((seq_t)current_k + 1) / 2;
        seq_t sigma = sigma_of(current_k);
        seq_t rem = T % sigma;            /* triangular mod sigma */

        /* If the remainder is prime, store k */
        if (is_prime_seq((uint64_t)rem)) {
            ensure_capacity(seq_len + 1, &seq_cap, OEISC_VPP(&seq_arr), sizeof(seq_t));
            seq_arr[seq_len++] = (seq_t)current_k;
        }
        ++current_k;
    }

    /* Return the requested term */
    return seq_arr[n - 1];
}
