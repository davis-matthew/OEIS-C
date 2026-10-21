/*
 * A266401  Self-inverse permutation of natural numbers: a(n) =
 *          A064989(A263273(A003961(n))).
 * https://oeis.org/A266401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A064989(A263273(A003961(n)))
 * Range:     u8 n <= 53, u16 n <= 2186, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   factor_u64, next_prime, prev_prime, reverse_b3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A003961 : shift prime factors of n one step towards larger primes */
/* A263273 : reverse the representation of n in standard base‑3 (digits 0‑2) */
/* A064989 : shift prime factors of n one step towards smaller primes */

#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/prev_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/reverse_b3.h"

/* Shift prime factors of an odd integer one step towards larger primes. */
static seq_t shift_primes_large(seq_t n)
{
    if (n <= 1) return n;
    seq_t res = 1;
    uint64_t primes[64], exps[64];
    size_t cnt;
    uint64_t n64 = (uint64_t)n;          /* assume n fits in 64 bits */
    factor_u64(n64, primes, exps, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = next_prime(primes[i]);   /* next prime > primes[i] */
        for (int e = 0; e < exps[i]; ++e)
            res *= (seq_t)q;
    }
    return res;
}

/* Shift prime factors of an odd integer one step towards smaller primes. */
static seq_t shift_primes_small(seq_t n)
{
    if (n <= 1) return n;
    seq_t res = 1;
    uint64_t primes[64], exps[64];
    size_t cnt;
    uint64_t n64 = (uint64_t)n;
    factor_u64(n64, primes, exps, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = prev_prime(primes[i]);   /* previous prime < primes[i] */
        for (int e = 0; e < exps[i]; ++e)
            res *= (seq_t)q;
    }
    return res;
}

/* Permutation applied to the odd part of n. */
static seq_t permute_odd(seq_t odd)
{
    seq_t t1 = shift_primes_large(odd);
    seq_t t2 = reverse_b3(t1);
    seq_t t3 = shift_primes_small(t2);
    return t3;
}

/* The requested sequence a(n).  */
seq_t a(idx_t n)
{
    /* Separate the power of two from the odd part. */
    idx_t k = 0;
    idx_t m = n;
    while ((m & 1u) == 0) {
        ++k;
        m >>= 1;
    }
    seq_t odd_part = (seq_t)m;

    seq_t odd_val = permute_odd(odd_part);

    /* Re‑attach the power of two. */
    seq_t two_pow = 1;
    for (idx_t i = 0; i < k; ++i)
        two_pow *= (seq_t)2;

    return two_pow * odd_val;
}
