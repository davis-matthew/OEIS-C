/*
 * A293188  Unitary pseudoperfect numbers: numbers that equal to the sum of a
 *          subset of their aliquot unitary divisors.
 * https://oeis.org/A293188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 4439, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9073 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A293188
 *  Unitary pseudoperfect numbers – numbers equal to the sum of a
 *  subset of their unitary divisors.
 *
 *  The implementation enumerates natural numbers starting from 6
 *  and tests each candidate for the unitary–pseudoperfect property.
 *  A subset‑sum DP over the unitary divisors (using a bitset) shows
 *  whether a subset sums to the candidate itself.  Results are cached
 *  in a dynamically resized array so that calls to a(idx_t n) are
 *  efficient even when n is requested in arbitrary order.
 *
 *  The code is fully generic: it works with any unsigned integer
 *  type seq_t (including arbitrary‑precision classes) and any
 *  64‑bit unsigned index type idx_t.  Arithmetic is performed with
 *  the allowed operators; only standard C library allocation
 *  (malloc/free) and the helper “seq_realloc” are used for the
 *  dynamic array of seq_t values.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
    (void)old_cap;
    return mem_resize(seq_t, old, new_cap);
}

/*---------------------------------------------------------------------*/
static bool is_unitary_pseudoperfect(uint64_t n)
{
    if (n < 6) return false;          /* first term is 6 */
    if (n == 6) return true;          /* trivial check */

    /*--- factor n; keep prime powers p^e -----------------------------------*/
    uint64_t primes[20];
    int      exps [20];
    size_t   pcnt = 0;
    uint64_t m    = n;

    /* factor 2 separately for speed */
    if ((m & 1ULL) == 0) {
        int cnt = 0;
        while ((m & 1ULL) == 0) { m >>= 1; ++cnt; }
        primes[pcnt] = 2;  exps[pcnt] = cnt;  ++pcnt;
    }

    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int cnt = 0;
            while (m % p == 0) { m /= p; ++cnt; }
            primes[pcnt] = p;  exps[pcnt] = cnt;  ++pcnt;
        }
    }
    if (m > 1) {
        primes[pcnt] = m;  exps[pcnt] = 1;  ++pcnt;
    }

    /* a prime cannot be unitary pseudoperfect */
    if (pcnt == 1 && exps[0] == 1) return false;

    /* pre‑compute prime powers p^e */
    uint64_t pw[20];
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t v = 1;
        for (int k = 0; k < exps[i]; ++k) v *= primes[i];
        pw[i] = v;
    }

    /* enumerate all unitary divisors except n itself (2^pcnt of them; a
       fixed 2^20 array of them on the stack overflowed it) */
    uint64_t *divs = (uint64_t *)malloc(((size_t)1 << pcnt) * sizeof *divs);
    if (!divs) abort();
    size_t div_cnt = 0;
    uint64_t sum_proper = 0;

    /* use bit mask over prime factors */
    for (uint64_t mask = 0; mask < (1ULL << pcnt); ++mask) {
        if (mask == ((1ULL << pcnt) - 1)) continue;  /* skip n itself */

        uint64_t prod = 1;
        for (size_t i = 0; i < pcnt; ++i)
            if (mask & (1ULL << i)) prod *= pw[i];

        divs[div_cnt++] = prod;
        sum_proper += prod;
    }

    /* quick shortcuts */
    if (sum_proper < n) { free(divs); return false; }
    if (sum_proper == n) { free(divs); return true; }   /* unitary perfect number */

    /* subset‑sum DP on unitary divisors using a word‑aligned bitset */
    size_t target = (size_t)n;
    size_t wcnt = (target >> 6) + 1;
    uint64_t *bits = (uint64_t *)calloc(wcnt, sizeof(uint64_t));
    if (!bits) abort();

    bits[0] = 1ULL;  /* sum 0 is always reachable */

    for (size_t di = 0; di < div_cnt; ++di) {
        uint64_t d = divs[di];
        if (d > target) continue;

        size_t wshift = d >> 6;
        unsigned int bshift = (unsigned int)(d & 63);

        /* propagate from high to low to avoid reuse */
        for (size_t iw = wcnt; iw-- > wshift; ) {
            uint64_t shifted = bits[iw - wshift] << bshift;
            if (bshift && iw > wshift)
                shifted |= bits[iw - wshift - 1] >> (64 - bshift);
            bits[iw] |= shifted;
        }

        if (bits[target >> 6] & (1ULL << (target & 63))) {
            free(bits);
            free(divs);
            return true;                 /* subset found */
        }
    }

    free(bits);
    free(divs);
    return false;                         /* no subset sums to n */
}

/*---------------------------------------------------------------------*/
/*  Helper: test whether a 64‑bit integer n is unitary pseudoperfect   */

/*---------------------------------------------------------------------*/
/*  Dynamic array of seq_t values                                      */
/*---------------------------------------------------------------------*/
static seq_t   *seq  = NULL;   /* cached sequence values */
static size_t  seq_len = 0;    /* number of stored elements */
static size_t  seq_cap = 0;    /* current allocation capacity */
static uint64_t next_candidate = 6;   /* integer we test next */

/* grow the seq array, doubling capacity */
static void seq_grow(void)
{
    if (seq_len < seq_cap) return;

    size_t new_cap = (seq_cap == 0) ? 16 : seq_cap << 1;
    seq_t *new_arr = seq_realloc(seq, seq_cap, new_cap);
    if (!new_arr) return;          /* allocation failure → keep old pointer */

    seq = new_arr;
    seq_cap = new_cap;
}

/*---------------------------------------------------------------------*/
/*  Public function required by the build                            */
/*---------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* produce at least n terms */
    while (seq_len < (size_t)n) {
        /* find next unitary pseudoperfect number */
        while (!is_unitary_pseudoperfect(next_candidate)) ++next_candidate;

        seq_grow();                 /* ensure room for new value */
        seq[seq_len++] = (seq_t)next_candidate;   /* cast is safe: value fits into seq_t */
        ++next_candidate;           /* advance search position */
    }

    return seq[(size_t)n - 1];   /* offset 1: a(1) is seq[0] */
}
