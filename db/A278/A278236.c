/*
 * A278236  Filter-sequence for factorial base (digit values): least number
 *          with the same prime signature as A276076(n).
 * https://oeis.org/A278236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A046523(A276076(n))
 * Range:     u8 n <= 22, u16 n <= 118, u32 n <= 2038, u64 n <= 9696,
 *            u128 n <= 9696, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the least integer whose prime
 * signature equals the multiset of non‑zero factorial‑base
 * digits of n, sorted in non‑increasing order and assigned
 * to successive primes. */
 
#include <stdint.h>
#include <stddef.h>

static const seq_t _p[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71
};
static const size_t _P_COUNT = sizeof(_p)/sizeof(_p[0]);

/* Convert n to factoradic digits, collect non‑zero digits
 * as exponents, sort them descending, then build the
 * minimal number with those exponents. */
seq_t a(idx_t n)
{
    /* At most 10‑k factorial base positions are needed for n<2^64,
       but for n up to 10k it is <20, safe to use fixed array. */
    uint8_t exps[20];
    size_t exps_cnt = 0;
    idx_t m = n;
    size_t pos = 0;
    while (m > 0) {
        uint64_t digit = m % (pos + 2);   /* digit ∈ [0, pos+1] */
        if (digit != 0) {
            exps[exps_cnt++] = (uint8_t)digit;
        }
        m /= (pos + 2);
        pos++;
    }

    /* sort exponents descending (simple bubble sort, small array) */
    for (size_t i = 0; i < exps_cnt; ++i)
        for (size_t j = i + 1; j < exps_cnt; ++j)
            if (exps[i] < exps[j]) {
                uint8_t tmp = exps[i];
                exps[i] = exps[j];
                exps[j] = tmp;
            }

    /* build the minimal number */
    seq_t result = 1;
    for (size_t i = 0; i < exps_cnt; ++i) {
        seq_t prime = _p[i];
        uint8_t e = exps[i];
        for (uint8_t k = 0; k < e; ++k)
            result *= prime;
    }
    return result;
}
