/*
 * A318883  Number of transient terms if unitary-proper-divisor-sum-function
 *          f(x) = A063919(x) is iterated and the initial value is n.
 * https://oeis.org/A318883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of transient part in iteration of the unitary
 *            proper divisor sum (A063919) starting at n
 * Range:     u8 n <= 1344, u16 n <= 87359, u32 n <= 87360, u64 n <= 87360,
 *            u128 n <= 87360, big any n
 * Validated: standard, pass, 121 terms through n = 9984 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation uses Floyd’s cycle‑finding algorithm and
    a trial‑division factorisation to compute A063919(x).  */

#include <stdint.h>
#include <stdbool.h>

/* Compute f(x) = sum of proper unitary divisors of x.
   For x = 1 the sum is defined as 1.  */
static seq_t f_unitary_proper_sum(seq_t n) {
    if (n == 1) return 1;

    /* Factorise n as uint64_t – valid for the range of n used
       in the tests (up to about 10^18) and sufficient for the
       transient/cycle detection required here. */
    uint64_t u   = (uint64_t)n;
    uint64_t tmp = u;
    seq_t prod   = 1;                 /* product over (1 + p^e) */

    /* factor 2 */
    unsigned int cnt = 0;
    while ((tmp & 1ULL) == 0) {
        ++cnt;
        tmp >>= 1;
    }
    if (cnt) {
        seq_t p_power = 1;
        for (unsigned int i = 0; i < cnt; ++i) p_power *= 2ULL;
        prod *= (1 + p_power);
    }

    /* factor odd primes */
    for (uint64_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            unsigned int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            seq_t p_power = 1;
            for (unsigned int i = 0; i < e; ++i) p_power *= (seq_t)p;
            prod *= (1 + p_power);
        }
    }
    if (tmp > 1)                     /* remaining prime factor */
        prod *= (1 + (seq_t)tmp);

    seq_t result = prod - (seq_t)u;
    return result;
}

/* Main sequence function */
seq_t a(idx_t n) {
    seq_t start = (seq_t)n;

    /* Floyd’s cycle‑finding */
    seq_t tortoise = start;
    seq_t hare     = f_unitary_proper_sum(start);
    while (tortoise != hare) {
        tortoise = f_unitary_proper_sum(tortoise);
        hare     = f_unitary_proper_sum(f_unitary_proper_sum(hare));
    }

    /* Find cycle length λ */
    idx_t cycle_len = 1;
    seq_t meet = f_unitary_proper_sum(tortoise);
    while (meet != tortoise) {
        meet = f_unitary_proper_sum(meet);
        ++cycle_len;
    }

    /* Find transient length μ (distance from start to cycle start) */
    tortoise = start;
    hare = start;
    for (idx_t i = 0; i < cycle_len; ++i)    /* advance hare λ steps */
        hare = f_unitary_proper_sum(hare);

    idx_t transient_len = 0;
    while (tortoise != hare) {
        tortoise = f_unitary_proper_sum(tortoise);
        hare     = f_unitary_proper_sum(hare);
        ++transient_len;
    }

    return (seq_t)transient_len;
}
