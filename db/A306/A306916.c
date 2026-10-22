/*
 * A306916  a(1)=1; for n > 1 replace each p^k in the prime factorization of n
 *          with prime(k)^p where prime(k) denotes the k-th prime number.
 * https://oeis.org/A306916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 36, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A306916:  a(n) = ∏ prime(k)^p   over all prime powers p^k in the 
 *                 factorisation of n, with a(1)=1
 */
#include <stdint.h>
#include <stddef.h>

/* first 200 prime numbers (1‑indexed) */
static const uint32_t _prime_table[200] = {
     2,   3,   5,   7,  11,  13,  17,  19,  23,  29,
    31,  37,  41,  43,  47,  53,  59,  61,  67,  71,
    73,  79,  83,  89,  97, 101, 103, 107, 109, 113,
   127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
   179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
   233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
   283, 293, 307, 311, 313, 317, 331, 337, 347, 349,
   353, 359, 367, 373, 379, 383, 389, 397, 401, 409,
   419, 421, 431, 433, 439, 443, 449, 457, 461, 463,
   467, 479, 487, 491, 499, 503, 509, 521, 523, 541,
   547, 557, 563, 569, 571, 577, 587, 593, 599, 601,
   607, 613, 617, 619, 631, 641, 643, 647, 653, 659,
   661, 673, 677, 683, 691, 701, 709, 719, 727, 733,
   739, 743, 751, 757, 761, 769, 773, 787, 797, 809,
   811, 821, 823, 827, 829, 839, 853, 857, 859, 863,
   877, 881, 883, 887, 907, 911, 919, 929, 937, 941,
   947, 953, 967, 971, 977, 983, 991, 997,1009,1013,
  1019,1021,1031,1033,1039,1049,1051,1061,1063,1069,
  1087,1091,1093,1097,1103,1109,1117,1123,1129,1151,
  1153,1163,1171,1181,1187,1193,1201,1213,1217
};

/* returns the k‑th prime (1‑indexed).  For k>200 a fallback is used. */
static inline seq_t kth_prime(idx_t k)
{
    if (k == 0) return (seq_t)1;
    if (k <= 200) return (seq_t)_prime_table[(size_t)k - 1];

    /* fallback for very large k (unlikely for n <= 2^64) */
    size_t found = 0;
    uint64_t candidate = 2;
    while (found < (size_t)k) {
        /* simple primality test */
        size_t i;
        for (i = 0; _prime_table[i] * (uint64_t)_prime_table[i] <= candidate; ++i) {
            if (candidate % _prime_table[i] == 0) break;
        }
        if (i == 0 || _prime_table[i] * (uint64_t)_prime_table[i] > candidate) {
            ++found;
            if (found == (size_t)k) return (seq_t)candidate;
        }
        ++candidate;
    }
    return (seq_t)0;   /* unreachable */
}

/* computes a(n) for the OEIS sequence A306916 */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    /* factorisation of n (trial division using primes up to 200) */
    uint64_t factors[64];
    uint64_t exps[64];
    size_t cnt = 0;
    uint64_t temp = (uint64_t)n;

    for (size_t i = 0; i < 200; ++i) {
        uint32_t p = _prime_table[i];
        if ((uint64_t)p * (uint64_t)p > temp) break;
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            factors[cnt] = (uint64_t)p;
            exps[cnt++]   = e;
        }
    }
    if (temp > 1) {          /* remaining prime factor */
        factors[cnt] = temp;
        exps[cnt++]   = 1;
    }

    /* compute the product prime(e_i)^p_i */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t base  = kth_prime((idx_t)exps[i]);     /* prime(k) */
        seq_t term  = (seq_t)1;
        idx_t exp   = (idx_t)factors[i];            /* p */
        for (idx_t j = 0; j < exp; ++j) term *= base;
        result *= term;
    }
    return result;
}
