/*
 * A385873  A sequence constructed by greedily sampling the Poisson
 *          distribution for parameter value 1 so as to minimize discrepancy.
 * https://oeis.org/A385873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A385873: greedy sampling from a Poisson(1) distribution.
   At step n choose the integer k (k ≥ 1) that maximises
       n * p(k) – c(k),   p(k) = 1/((e-1)·k!) ,
   where c(k) is the number of occurrences of k among a(1)…a(n‑1).
   The returned value a(n) is the chosen k. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

/* Parameters for the Poisson(1) distribution.  λ = 1,  e = 2.718281828… */
#define MAX_K 50                          /* safe upper bound for k up to n≈10000 */
static const long double E_MINUS1 = 2.71828182845904523536L - 1.0L;

/* ------------------------------------------------------------------ */
/*   State that is lazily created on the first call to a()              */
/* ------------------------------------------------------------------ */

static seq_t *seq_arr      = NULL;        /* 1‑indexed array storing the sequence */
static size_t seq_capacity = 0;           /* current capacity of seq_arr        */
static idx_t seq_len       = 0;           /* largest n that has been computed    */

static int  counts[MAX_K + 1] = {0};      /* c(k) for k = 0…MAX_K (c(0) unused) */
static long double fact[MAX_K + 1]  = {0};/* factorials 0! … MAX_K!              */
static long double denom[MAX_K + 1] = {0};/* (e-1)·k! for k = 0…MAX_K           */

/* ------------------------------------------------------------------ */
/*   One‑time initialisation of factorials and denominators            */
/* ------------------------------------------------------------------ */
static void init_once(void)
{
    static bool done = false;
    if (done) return;
    fact[0] = 1.0L;
    for (int k = 1; k <= MAX_K; ++k) {
        fact[k] = fact[k-1] * (long double)k;
        denom[k] = E_MINUS1 * fact[k];
    }
    memset(counts, 0, sizeof(counts));
    done = true;
}

/* ------------------------------------------------------------------ */
/*   Ensure seq_arr has capacity for indices up to n (0‑based          */
/*   indices are stored at seq_arr[1] … seq_arr[n]                    */
/* ------------------------------------------------------------------ */
static void ensure_capacity(idx_t n)
{
    if (seq_arr == NULL || seq_capacity <= (size_t)n) {
        size_t new_cap = seq_capacity ? seq_capacity * 2 : 256;
        if (new_cap < (size_t)(n + 1)) new_cap = (size_t)(n + 1);
        seq_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
        seq_capacity = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/*   Generate the sequence up to index n (inclusive)                   */
/* ------------------------------------------------------------------ */
static void generate_upto(idx_t n)
{
    init_once();
    ensure_capacity(n);

    for (idx_t i = seq_len + 1; i <= n; ++i) {
        long double best   = -1e300L;
        int        best_k  = 1;

        /* Evaluate the greedy criterion for all k up to MAX_K. */
        for (int k = 1; k <= MAX_K; ++k) {
            long double expected = (long double)i / denom[k];
            long double diff     = expected - (long double)counts[k];
            if (diff > best) {
                best  = diff;
                best_k = k;
            }
        }
        seq_arr[(size_t)i] = (seq_t)best_k;
        counts[best_k] += 1;
    }
    seq_len = n;
}

/* ------------------------------------------------------------------ */
/*   Public interface required by the build system                     */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n > seq_len) generate_upto(n);
    return seq_arr[(size_t)n];
}
