/*
 * A337533  1 together with nonsquares whose square part's square root is in
 *          the sequence.
 * https://oeis.org/A337533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 229, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A337533 – “1 together with nonsquares whose square part's
   square root is in the sequence”.
   Each term a(n) is obtained exactly from the definition:
   a(1)=1; for n>1, a(n) is included iff
       1. n is not a perfect square,
       2. let s be the largest square dividing n,
       3. sqrt(s) (i.e. the square part's square root) is already
          known to be a term of the sequence. */
   
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* External types supplied by the build system */
typedef uint64_t idx_t;   /* unsigned integer index */

/* Forward declarations for helper header files - unused here, but
   kept so the file compiles under the build environment.  */
#include "../utils/gcd.h"   /* just an example; not actually called */

/* ----------------------------------------------------------------------- */
/* Static state for memoised computation                                   */
/* ----------------------------------------------------------------------- */
static seq_t *seq      = NULL;    /* array of sequence values */
static size_t seq_len  = 0;       /* number of values stored */
static size_t seq_cap  = 0;       /* capacity of seq array */

static idx_t *spf       = NULL;   /* smallest prime factor table  */
static idx_t  spf_limit = 0;      /* highest index for which spf is valid */

static uint8_t *in_seq    = NULL; /* 1 if number is in the sequence   */
static idx_t    in_limit  = 0;     /* maximum index for which in_seq is valid */

/* ----------------------------------------------------------------------- */
/* Smallest‑prime‑factor sieve                                              */
/* ----------------------------------------------------------------------- */
static void build_spf(idx_t limit)
{
    idx_t *arr = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    if (!arr) return;                 /* out‑of‑memory – should not happen for limits we use */

    memset(arr, 0, (limit + 1) * sizeof(idx_t));
    arr[0] = 0;
    arr[1] = 1;

    for (idx_t i = 2; i <= limit; ++i) {
        if (arr[i] == 0) {
            arr[i] = i;                     /* i is prime */
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i) {
                    if (arr[j] == 0) arr[j] = i;
                }
            }
        }
    }
    if (spf) free(spf);
    spf = arr;
    spf_limit = limit;
}

/* ----------------------------------------------------------------------- */
/* Extend the precomputation workspace up to 'limit'.                      */
/* After this call, in_seq[1..limit] is valid and seq[] contains all       */
/* sequence terms ≤ limit (in increasing order).                           */
/* ----------------------------------------------------------------------- */
static void extend(idx_t limit)
{
    if (limit <= in_limit) return;          /* already computed up to this point */

    /* Rebuild the spf table if necessary */
    if (limit > spf_limit) {
        build_spf(limit);
    }

    /* Grow the in_seq bitmap */
    uint8_t *new_in = (uint8_t *)realloc(in_seq, (limit + 1) * sizeof(uint8_t));
    if (!new_in) return;                    /* memory failure */
    if (in_limit == 0) {
        memset(new_in, 0, (limit + 1) * sizeof(uint8_t));
    } else {
        memset(new_in + in_limit + 1, 0,
               (limit - in_limit) * sizeof(uint8_t));
    }
    in_seq = new_in;

    /* The very first term */
    if (in_limit == 0) {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len = 1;
        seq[0]  = (seq_t)1;                 /* a(1) = 1 */
        in_seq[1] = 1;                       /* 1 is in the sequence */
        in_limit = 1;
    }

    /* Process numbers from in_limit+1 up to limit */
    for (idx_t m = in_limit + 1; m <= limit; ++m) {
        idx_t temp = m;
        bool  is_square = true;             /* true if all exponents even */
        seq_t sqrt_part = 1;                /* sqrt of the largest square dividing m */

        while (temp > 1) {
            idx_t p = spf[temp];
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            if (e & 1) is_square = false;   /* odd exponent ⇒ m is not a perfect square */
            if (e >= 2) {
                /* multiply by p^{floor(e/2)} to build sqrt_part */
                seq_t pow_p = 1;
                for (idx_t k = 0; k < e / 2; ++k) pow_p *= (seq_t)p;
                sqrt_part *= pow_p;
            }
        }

        /* Exclude perfect squares (except 1, already handled) */
        if (!is_square && in_seq[(idx_t)sqrt_part]) {
            in_seq[m] = 1;
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)m;
        }
    }
    in_limit = limit;
}

/* ----------------------------------------------------------------------- */
/* Public API – returns the n‑th term of the sequence (a(1)=1, n≥1).       */
/* The function is safe to call in any order and will memoise results.  */
/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, return 0 as sentinel */

    /* Ensure that the precomputation contains at least n terms. */
    /* If seq_len < n, we extend the range until we have at least n terms. */
    if (seq_len < n) {
        idx_t estimate = in_limit == 0 ? 64 : in_limit;
        while (seq_len < n) {
            if (estimate < n * 2) estimate = n * 2;
            extend(estimate);
            estimate *= 2;
        }
    }
    return seq[n - 1];
}
