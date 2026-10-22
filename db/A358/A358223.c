/*
 * A358223  Inverse Möbius transform of A181819, prime shadow function.
 * https://oeis.org/A358223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} A181819(d). Multiplicative with a(p^e) = 1 +
 *            Sum_{k=1..e} prime(k)
 * Range:     u8 n <= 479, u16 n <= 9914, u32 n <= 9914, u64 n <= 9914,
 *            u128 n <= 9914, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* first 64 primes */
static const uint64_t prime_list[] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,
    59,61,67,71,73,79,83,89,97,101,103,107,109,113,
    127,131,137,139,149,151,157,163,167,173,179,181,
    191,193,197,199,211,223,227,229,233,239,241,251,
    257,263,269,271,277,281,283,293,307,311
};
#define PRIME_LIST_LEN (sizeof(prime_list)/sizeof(prime_list[0]))

/* compute 1 + sum of first e primes */
static inline seq_t value_for_exp(int e){
    seq_t v = 1;
    for (int i = 0; i < e; ++i){
        v += (seq_t)prime_list[i];
    }
    return v;
}

seq_t a(idx_t n){
    if (n == 0) return 0;          /* undefined below offset, but guard */
    seq_t res = 1;
    uint64_t m = (uint64_t)n;

    /* factor 2 */
    int exp = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        exp++;
    }
    if (exp > 0) res *= value_for_exp(exp);

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            res *= value_for_exp(exp);
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (m > 1) {
        res *= value_for_exp(1);
    }

    return res;
}
