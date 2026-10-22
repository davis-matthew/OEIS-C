/*
 * A363033  Sum of divisors of 5*n-1 of form 5*k+3.
 * https://oeis.org/A363033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of divisors d of (5*n-1) with d ≡ 3 (mod 5)
 * Range:     u8 n <= 51, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* compute A363033(n) for n >= 1 */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* safety for offset */
    /* m = 5*n - 1 */
    idx_t m = 5 * n - 1;
    /* factor m into primes */
    idx_t primes[64];
    idx_t exps[64];
    size_t pf_cnt = 0;

    /* factor 2 */
    size_t cnt = 0;
    while ((m & 1ULL) == 0) {
        ++cnt;
        m >>= 1;
    }
    if (cnt) {
        primes[pf_cnt] = 2;
        exps[pf_cnt] = cnt;
        ++pf_cnt;
    }

    /* factor odd primes */
    for (idx_t p = 3; p * p <= m; p += 2) {
        cnt = 0;
        while (m % p == 0) {
            ++cnt;
            m /= p;
        }
        if (cnt) {
            primes[pf_cnt] = p;
            exps[pf_cnt] = cnt;
            ++pf_cnt;
        }
    }
    if (m > 1) {
        primes[pf_cnt] = m;
        exps[pf_cnt] = 1;
        ++pf_cnt;
    }

    /* enumerate all divisors */
    seq_t *divs = (seq_t*)malloc(sizeof(seq_t));
    if (!divs) return 0;                 /* allocation failure safety */
    divs[0] = 1;
    size_t div_len = 1;

    for (size_t i = 0; i < pf_cnt; ++i) {
        idx_t p = primes[i];
        size_t e = exps[i];
        size_t cur_len = div_len;
        for (size_t j = 0; j < cur_len; ++j) {
            seq_t base = divs[j];
            seq_t mult = 1;
            for (size_t k = 1; k <= e; ++k) {
                mult *= (seq_t)p;
                /* grow array if needed */
                divs = (seq_t*)realloc(divs, sizeof(seq_t) * (div_len + 1));
                divs[div_len++] = base * mult;
            }
        }
    }

    /* sum divisors congruent to 3 mod 5 */
    seq_t sum = 0;
    for (size_t i = 0; i < div_len; ++i) {
        if (divs[i] % 5 == 3)
            sum += divs[i];
    }
    free(divs);
    return sum;
}
