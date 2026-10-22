/*
 * A333195  Numbers with three consecutive prime indices in arithmetic
 *          progression.
 * https://oeis.org/A333195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer whose multiset of prime indices (the
 *            indices of the primes dividing it) contains a length‑3
 *            arithmetic progression. The function is memoised so calls in any
 *            order are fast
 * Range:     u8 n <= 49, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* The build supplies idx_t and seq_t.  */
static uint32_t *spf   = NULL;          /* smallest prime factor for 0..limit */
static uint32_t *pidx  = NULL;          /* prime → (1‑based) index */
static size_t    prime_cnt = 0;
static uint32_t  max_sieve = 2000000u;   /* value large enough for n≤10000 */

/* linear sieve initialisation */
static void init_sieve(void)
{
    if (spf != NULL) return;           /* already initialised */

    spf  = (uint32_t*)calloc(max_sieve + 1, sizeof(uint32_t));
    pidx = (uint32_t*)calloc(max_sieve + 1, sizeof(uint32_t));
    if (!spf || !pidx) exit(1);        /* out of memory */

    static uint32_t primes_arr[2000000 / 10 + 1];
    size_t primes_sz = 0;

    for (size_t i = 2; i <= max_sieve; ++i) {
        if (spf[i] == 0) {             /* i is prime */
            spf[i]   = (uint32_t)i;
            pidx[i]  = (uint32_t)(++prime_cnt);
            primes_arr[primes_sz++] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_sz; ++j) {
            uint32_t p = primes_arr[j];
            if (p > spf[i] || (uint64_t)i * p > max_sieve) break;
            spf[i * p] = p;
        }
    }
}

/* test whether a number has a length‑3 arithmetic progression in its
   sorted multiset of prime indices. */
static bool qualifies(uint32_t n)
{
    /* indices array is small (≤64 entries) for n≤max_sieve */
    uint32_t indices[64];
    size_t   len = 0;
    while (n > 1) {
        uint32_t p   = spf[n];
        uint32_t exp = 0;
        while (n % p == 0) {
            n /= p;
            ++exp;
        }
        uint32_t idx = pidx[p];
        /* push idx exp times (indices are already sorted) */
        for (uint32_t i = 0; i < exp; ++i) {
            if (len < sizeof(indices)/sizeof(indices[0]))
                indices[len++] = idx;
            else
                return false;          /* safety: should not happen */
        }
    }
    for (size_t i = 0; i + 2 < len; ++i) {
        uint32_t d1 = indices[i + 1] - indices[i];
        uint32_t d2 = indices[i + 2] - indices[i + 1];
        if (d1 == d2) return true;
    }
    return false;
}

/* memoised sequence storage */
static seq_t *seq  = NULL;
static size_t seq_len = 0, seq_cap = 0;
static uint32_t next_val = 2;          /* start checking from 2 */

/* append value to the memoised sequence */
static void seq_append(seq_t val)
{
    if (seq_cap == seq_len) {
        size_t new_cap = seq_cap ? (seq_cap << 1) : 4;
        seq_t *new_arr = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
        if (!new_arr) exit(1);
        seq = new_arr;
        seq_cap = new_cap;
    }
    seq[seq_len++] = val;
}

/* public function required by the build */
seq_t a(idx_t n)
{
    static bool once = false;
    if (!once) {
        init_sieve();        /* set up spf and prime indices */
        once = true;
    }
    while (seq_len < (size_t)n) {
        if (next_val <= max_sieve && qualifies((uint32_t)next_val))
            seq_append((seq_t)next_val);
        ++next_val;
    }
    return seq[(size_t)n - 1];
}
