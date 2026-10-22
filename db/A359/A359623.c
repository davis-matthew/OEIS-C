/*
 * A359623  a(n) is the least integer of the form sum(X)/sum(Y) where {X, Y}
 *          runs through the partitions of the divisors of n into two nonempty
 *          sets (and sum(Z) is the sum of the elements of Z).
 * https://oeis.org/A359623
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest k such that k+1 | S(n) and the sum k·S/(k+1)
 *            is attainable as a subset sum of the proper divisors of n. Here
 *            S(n) is the sum of all divisors of n
 * Range:     u8 n <= 16, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Simple integer sqrt for unsigned 64‑bit integers, using binary search. */
static uint64_t uint64_isqrt(uint64_t n)
{
    uint64_t low = 0, high = n, mid, best = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        if (mid > 0 && mid > n / mid) {
            high = mid - 1;
        } else {
            best = mid;
            low = mid + 1;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    /* Handle the offset: for n < 2 the result is irrelevant. */
    if (n < 2) return 0;

    /* --------------------------------- 1. Divisors of n --------------------------------- */
    /* We'll keep the divisors in a small array; n <= 1e4 ⇒ at most ~64 divisors. */
    uint64_t divs[128];
    size_t div_cnt = 0;
    for (uint64_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            divs[div_cnt++] = i;
            if (i != n / i) {
                divs[div_cnt++] = n / i;
            }
        }
    }

    /* --------------------------------- 2. Sum of divisors S --------------------------------- */
    seq_t S = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        S += (seq_t)divs[i];
    }

    /* --------------------------------- 3. Subset sum DP --------------------------------- */
    /* We need a bitset with one bit per sum value up to S. */
    size_t S_idx = (size_t)S;                         /* S fits in size_t for our n range */
    size_t words = (S_idx >> 6) + 2;                   /* +2 for guard against overflow */
    uint64_t *bits  = (uint64_t *)calloc(words, sizeof(uint64_t));
    uint64_t *newbits = (uint64_t *)calloc(words, sizeof(uint64_t));
    if (!bits || !newbits) {                          /* allocation failure: give up */
        free(bits); free(newbits);
        return 0;
    }
    bits[0] = 1ULL;                                   /* sum 0 is always reachable */

    for (size_t di = 0; di < div_cnt; ++di) {
        uint64_t val = divs[di];
        size_t whole = val >> 6;                       /* val / 64          */
        size_t rem   = val & 63;                       /* val % 64          */

        /* Copy current bits to newbits */
        memcpy(newbits, bits, words * sizeof(uint64_t));

        /* Shift bits by 'val' and OR into newbits */
        for (size_t i = whole; i < words; ++i) {
            uint64_t shifted = bits[i - whole] << rem;
            if (rem && i - whole >= 1) {
                shifted |= bits[i - whole - 1] >> (64 - rem);
            }
            newbits[i] |= shifted;
        }

        /* Prepare for next divisor */
        uint64_t *tmp = bits;
        bits = newbits;
        newbits = tmp;
    }

    /* --------------------------------- 4. Divisors of S --------------------------------- */
    /* S is small (≤ ~5·10⁷ for n ≤ 10⁴), so we can safely cast to uint64_t. */
    uint64_t S_big = (uint64_t)S;
    uint64_t sqrtS = uint64_isqrt(S_big);

    uint64_t d_limit = 128;
    uint64_t *sdivs = (uint64_t *)malloc(d_limit * sizeof(uint64_t));
    size_t sdiv_cnt = 0;

    for (uint64_t i = 1; i <= sqrtS; ++i) {
        if (S_big % i == 0) {
            if (sdiv_cnt >= d_limit) {
                d_limit *= 2;
                sdivs = (uint64_t *)realloc(sdivs, d_limit * sizeof(uint64_t));
            }
            sdivs[sdiv_cnt++] = i;
            uint64_t j = S_big / i;
            if (j != i) {
                if (sdiv_cnt >= d_limit) {
                    d_limit *= 2;
                    sdivs = (uint64_t *)realloc(sdivs, d_limit * sizeof(uint64_t));
                }
                sdivs[sdiv_cnt++] = j;
            }
        }
    }

    /* --------------------------------- 5. Find minimal k --------------------------------- */
    seq_t best_k = S;   /* dummy initial value (will always be replaced) */
    bool found = false;

    for (size_t idx = 0; idx < sdiv_cnt; ++idx) {
        uint64_t d = sdivs[idx];
        if (d <= 1) continue;            /* skip d = 1 → k = 0 */
        uint64_t k = d - 1;
        /* Compute target sum = k * S / d */
        seq_t product = (seq_t)k * S;
        seq_t target = product / (seq_t)d;

        /* Eliminate empty or full partitions */
        if (target == 0 || target == S) continue;

        /* Check whether target sum is achievable via subset sums. */
        size_t t = (size_t)target;
        size_t w = t >> 6;
        size_t b = t & 63;
        if (w < words && (bits[w] & (1ULL << b))) {
            if (!found || (seq_t)k < best_k) {
                best_k = (seq_t)k;
                found = true;
            }
        }
    }

    /* --------------------------------- 6. Cleanup --------------------------------- */
    free(bits);
    free(newbits);
    free(sdivs);

    /* We should always find a value, but if not, default to S. */
    return found ? best_k : S;
}
