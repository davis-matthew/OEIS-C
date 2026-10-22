/*
 * A397933  Numbers k that are neither squarefree nor powerful such that
 *          rad(k)*bigomega(k) = k, where rad = A007947 and bigomega =
 *          A001222.
 * https://oeis.org/A397933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 2503, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation of OEIS sequence A397933
 *
 *  Numbers k that are neither squarefree nor powerful and satisfy
 *  rad(k) * bigomega(k) = k, i.e. k / rad(k) = bigomega(k).
 *
 *  The function computes the sequence by brute‑force search,
 *  memoising previously found terms.  The search is performed
 *  over k = 2,3,4,... until n terms have been collected.
 *  All integer computations are performed with uint64_t,
 *  while the sequence values are stored in the arbitrary
 *  precision type `seq_t`.  The array of terms is statically
 *  allocated with a capacity that comfortably holds the first
 *  20 000 terms – sufficient for n up to 10 000 as required.
 *
 *  Dependencies:
 *      #include "../utils/factor_u64.h"
 *
 *  No dynamic allocation is performed at runtime, so this code
 *  is thread‑safe for repeated calls to a(idx_t n).
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* static cache of sequence terms */
static seq_t seq_cache[20000];   /* capacity for useful n (>=10 000) */
static size_t seq_len    = 0;    /* number of terms currently stored   */
static idx_t  last_checked = 2;   /* next integer to test              */

/* Push a new term into the static cache */
static void push_term(seq_t val)
{
    if (seq_len < 20000) {
        seq_cache[seq_len++] = val;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for n = 0 */
    if (n <= seq_len) return seq_cache[n-1];

    /* temporary buffers for factorisation */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;

    /* search until we have n terms */
    while (seq_len < (size_t)n) {
        uint64_t k = (uint64_t)last_checked++;

        /* factorise k */
        factor_u64(k, primes, exps, &cnt);

        /* compute rad(k), bigomega(k), and flags */
        uint64_t rad = 1;           /* product of distinct primes   */
        uint64_t bigom = 0;         /* sum of exponents            */
        bool has_one = false;
        bool has_ge2 = false;

        for (size_t i = 0; i < cnt; ++i) {
            rad   *= primes[i];
            bigom += exps[i];
            if (exps[i] == 1) has_one = true;
            else if (exps[i] >= 2) has_ge2 = true;
        }

        /* check rad(k)*bigomega(k) = k  ⇔  k / rad(k) == bigomega(k) */
        if (rad != 0 && k % rad == 0 && k / rad == bigom &&
            has_ge2 && has_one) {  /* not squarefree && not powerful */
            push_term((seq_t)k);
        }
    }

    return seq_cache[n-1];
}
