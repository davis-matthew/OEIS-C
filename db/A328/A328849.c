/*
 * A328849  Numbers in whose primorial base expansion only even digits appear.
 * https://oeis.org/A328849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2 * Σ digit_k · primorial_k where digit_k is chosen by *
 *            expressing n–1 in the mixed radix system with radices *
 *            choices_k = (p_{k+1}+1)/2 * (p_k is the k‑th prime). The digits
 *            correspond to the even digits of * the primorial base expansion,
 *            so a(n) is twice the number obtained by * treati...
 * Range:     u8 n <= 24, u16 n <= 1203, u32 n <= 9072, u64 n <= 9072,
 *            u128 n <= 9072, big any n
 * Validated: standard, pass, 84 terms through n = 8987 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)0;                  /* a(1) = 0 */

    idx_t idx = n - (idx_t)1;             /* zero‑based mixed‑radix index */
    seq_t half = (seq_t)0;                /* H = Σ digit_k · primorial_k   */
    seq_t mult  = (seq_t)1;               /* primorial_k  (P_k)             */
    idx_t k = (idx_t)0;                   /* position counter */

    while (idx != (idx_t)0)
    {
        idx_t prime     = get_prime(k + (idx_t)1);          /* p_{k+1}   */
        seq_t choices_s = (seq_t)(prime + (idx_t)1) / (seq_t)2;
        idx_t choices   = (idx_t)choices_s;                /* (p_{k+1}+1)/2 */

        idx_t digit_index = idx % choices;
        idx /= choices;

        half += (seq_t)digit_index * mult;                 /* contribute */

        mult *= prime;                                    /* next primorial */
        ++k;
    }

    return half * (seq_t)2;                               /* a(n) = 2·half   */
}
