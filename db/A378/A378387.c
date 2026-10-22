/*
 * A378387  a(n) is the number of proper divisors d of n such that (-d)^n ==
 *          -d (mod n).
 * https://oeis.org/A378387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of proper divisors d of n such that (-d)^n ≡
 *            -d (mod n). (OEIS A378387)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute x^e mod m for non‑negative integers using 128‑bit
   multiplication to avoid overflow. */
static uint64_t pow_mod_uint64(uint64_t x, uint64_t e, uint64_t m)
{
    uint64_t res = 1 % m;
    uint64_t base = x % m;
    while (e) {
        if (e & 1ULL)
            res = (uint64_t)(((__int128)res * base) % m);
        base = (uint64_t)(((__int128)base * base) % m);
        e >>= 1;
    }
    return res;
}

/* Count the proper divisors d of n satisfying (-d)^n ≡ -d (mod n). */
seq_t a(idx_t n)
{
    if (n == 1U)            /* no proper divisors */
        return (seq_t)0;

    uint64_t N = (uint64_t)n;   /* working in 64‑bit for speed */
    seq_t  count = 0;

    /* Enumerate all proper divisors d of N. */
    for (uint64_t d = 1; d < N; ++d) {
        if (N % d != 0)
            continue;

        /* Compute (-d)^N modulo N. */
        uint64_t powd = pow_mod_uint64(d, N, N);     /* d^N mod N */

        uint64_t left;   /* value of (-d)^N mod N */
        if (N % 2ULL == 0ULL)                          /* even N: (-d)^N = d^N */
            left = powd % N;
        else                                            /* odd N: (-d)^N = -d^N */
            left = (N - powd) % N;                 /* (-powd) mod N */

        /* Right side: -d mod N = N - d. */
        uint64_t right = N - d;

        if (left == right)
            ++count;
    }

    return count;
}
