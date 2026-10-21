/*
 * A286466  Compound filter: a(n) = P(A112049(n), A046523(n)), where P(n,k) is
 *          sequence A000027 used as a pairing function.
 * https://oeis.org/A286466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2)*(2 + ((A112049(n)+A046523(n))^2) - A112049(n) -
 *            3*A046523(n))
 * Range:     u8 n <= 15, u16 n <= 359, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime, jacobi_symbol, pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/factor_u64.h"
#include "../utils/jacobi_symbol.h"
#include "../utils/pow_uint.h"
#include "../utils/seq_mul.h"
#include "../utils/is_prime.h"

/* Small primes used to build the "signature" of n */
static const uint64_t PLIST[] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,
    73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,
    157,163,167,173,179,181,191,193,197,199,211,223,227,229,233,
    239,241,251,257,263,269,271,277,281,283,293,307,311,313,317,
    331,337,347,349,353,359,367,373,379,383,389,397,401,409,419,
    421,431,433,439,443,449,457,461,463,467,479,487,491,499,503,
    509,521,523,541,547,557,563,569,571,577,587,593,599,601,607,
    613,617,619,631,641,643,647,653,659,661,673,677,683,691,701,
    709,719,727,733,739,743,751,757,761,769,773
};
#define PLIST_SIZE (sizeof(PLIST)/sizeof(PLIST[0]))

/* Comparator for sorting exponents in descending order */
static int cmp_desc(const void *a, const void *b) {
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    if (va < vb) return 1;
    if (va > vb) return -1;
    return 0;
}

/* Compute the “prime signature” of n: the minimal integer having the same multiset
 * of exponents as n, obtained by assigning the largest exponents to the smallest
 * primes 2,3,5,… and multiplying the resulting powers. */
static seq_t prime_signature(idx_t n) {
    uint64_t pf[64], exp[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pf, exp, &cnt);

    if (cnt > 1) {
        qsort(exp, cnt, sizeof(uint64_t), cmp_desc);
    }

    seq_t sig = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = pow_uint(PLIST[i], (idx_t)exp[i]);
        sig = seq_mul(sig, term);
    }
    return sig;
}

/* Find the index (1‑based) of the first prime p such that
 * Jacobi(p, 2n+1) ≠ +1 (i.e. 0 or –1). */
static idx_t first_prime_index(idx_t n) {
    uint64_t m = 2 * (uint64_t)n + 1;
    idx_t idx = 1;
    for (uint64_t p = 2; ; ++p) {
        if (is_prime((idx_t)p)) {
            if (jacobi_symbol(p, m) != 1) {
                return idx;
            }
            ++idx;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for n<1, but safe fallback */

    idx_t x_idx = first_prime_index(n);
    seq_t x = (seq_t)x_idx;
    seq_t y = prime_signature(n);

    seq_t sum = x + y;
    seq_t num = sum * sum - x - (seq_t)3 * y + (seq_t)2;
    return num / (seq_t)2;
}
