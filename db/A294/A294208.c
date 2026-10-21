/*
 * A294208  a(n) = reduced numerator of Sum_{p <= n}
 *          Sum_{k=1..floor(log(n)/log(p))} 1/p^k, where p runs over the
 *          primes.
 * https://oeis.org/A294208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = reduced numerator of Sum_{p <= n}
 *            Sum_{k=1..floor(log(n)/log(p))} 1/p^k Equivalent: for each prime
 *            p <= n let m = floor(log(n)/log(p)); term = (p^m-1) /
 *            (p^m*(p-1)) = (1 + p + ... + p^{m-1}) / p^m. Sum all terms and
 *            return the reduced numerator
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 42, u128 n <= 88,
 *            big any n
 * Helpers:   big_pow, build_primes, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/big_pow.h"
#include "../utils/build_primes.h"

seq_t a(idx_t n) {
    /* Handle the offset: a(0) = 0, a(1) = 0 */
    if (n < 2) return 0;

    /* Cache of primes up to the current maximum n seen */
    static idx_t *prime_list = NULL;
    static size_t prime_cnt  = 0;
    static idx_t  prime_limit = 0;

    if (n > prime_limit) {
        /* Rebuild the prime list up to n */
        free(prime_list);
        build_primes(n, &prime_list, &prime_cnt);
        prime_limit = n;
    }

    seq_t sum_num = 0;   /* Numerator of the running sum */
    seq_t sum_den = 1;   /* Denominator of the running sum */

    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if (p > n) break;

        /* Compute m = floor(log(n)/log(p)) by repeated multiplication */
        idx_t m = 1;
        idx_t pow_idx = p;
        if (pow_idx == 0) continue;     /* defensive, should not happen */
        while (pow_idx <= n / p) {
            pow_idx *= p;
            ++m;
        }

        /* Compute p^m as a seq_t */
        seq_t pow_seq = big_pow((seq_t)p, (seq_t)m);

        /* Reduced term: numerator = (p^m - 1)/(p-1), denominator = p^m */
        seq_t term_num = (pow_seq - 1) / (seq_t)(p - 1);
        seq_t term_den = pow_seq;

        /* Add term to the running sum: sum_num/sum_den + term_num/term_den */
        seq_t new_num = sum_num * term_den + term_num * sum_den;
        seq_t new_den = sum_den * term_den;

        /* Reduce the fraction */
        seq_t g = gcd(new_num, new_den);
        sum_num = new_num / g;
        sum_den = new_den / g;
    }

    return sum_num;
}
