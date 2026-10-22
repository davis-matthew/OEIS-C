/*
 * A331302  Number of 4k+3 composites encountered when traversing from n to
 *          the root of A005940-tree.
 * https://oeis.org/A331302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9901, u16 n <= 9901, u32 n <= 9901, u64 n <= 9901,
 *            u128 n <= 9901, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A331302
 *  a(1) = 0; a(p) = 0 for primes;
 *  a(n) = [n ≡ 3 (mod 4)] + a(A252463(n))   for composite n
 *  where A252463(n) = n/2   if n is even
 *  and      = ∏ prev(p)^e  if n is odd,
 *  p^e a prime power in the factorisation of n and
 *  prev(p) is the largest prime < p (prev(3)=2).
 *
 *  The implementation below evaluates the map once per step,
 *  counts composite terms ≡3 mod 4 on the trajectory
 *  n → A252463(n) → … → 1, and returns that count.
 */

#include <stdint.h>
#include <stdbool.h>

/* ---------- Helper: primality test (trial division) ---------- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)           return false;
    if (n % 2ULL == 0ULL) return n == 2ULL;
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL)
        if (n % d == 0ULL)
            return false;
    return true;
}

/* ---------- Helper: previous prime (largest prime < p) ---------- */
static uint64_t prev_prime_uint64(uint64_t p)
{
    if (p <= 2ULL) return 2ULL;        /* never used for p<3, but guard */
    for (uint64_t q = p - 1ULL; q >= 2ULL; --q)
        if (is_prime_uint64(q))
            return q;
    return 2ULL;                       /* fallback, should not occur */
}

/* ---------- A252463 map (single step) ---------- */
static idx_t map252463(idx_t n)
{
    if ((n & 1ULL) == 0ULL)            /* even: divide by 2 */
        return n >> 1;
    /* odd: factorise and replace each prime factor by its predecessor */
    idx_t tmp  = n;
    idx_t prod = 1;
    while (tmp > 1ULL) {
        /* find smallest prime factor of tmp */
        uint64_t p = 0;
        for (uint64_t d = 2ULL; d * d <= tmp; ++d) {
            if (tmp % d == 0ULL) { p = d; break; }
        }
        if (p == 0ULL) { p = tmp; }    /* tmp itself is prime */
        int exp = 0;
        while (tmp % p == 0ULL) { tmp /= p; ++exp; }
        uint64_t prev = prev_prime_uint64(p);
        unsigned __int128 pow = 1;
        for (int i = 0; i < exp; ++i) pow *= prev;
        prod = (idx_t)((unsigned __int128)prod * pow);
    }
    return prod;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t cur = n;
    while (cur > 1ULL) {
        if (!is_prime_uint64(cur) && ((cur & 3ULL) == 3ULL))
            res += 1;
        cur = map252463(cur);
    }
    return res;
}
