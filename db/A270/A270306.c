/*
 * A270306  Smallest number which is representable as a sum of n but no fewer
 *          consecutive nonnegative integers, or 0 for n > 1 if no such number
 *          exists.
 * https://oeis.org/A270306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest integer representable as a sum of n
 *            consecutive nonnegative integers but not representable as a sum
 *            of any smaller number of consecutive nonnegative integers.
 *            Sequence values are zero for n>1 if no such integer exists
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following implementation uses number‑theoretic observations:
   - If n is 1 → 0.
   - If n is 2 → 1.
   - If n is a power of two (n >= 4) → a(n) = (n/2) * p,
     where p is the smallest odd prime greater than n.
   - If n is an odd prime → a(n) = 2^⌊log₂(n‑1)⌋ * n.
   - Otherwise a(n) = 0. */

#include <stdbool.h>
#include <stdint.h>

/* ---------- Helper functions ------------------------------------ */
static bool is_prime(idx_t x)
{
    if (x < 2) return false;
    if (x == 2 || x == 3) return true;
    if ((x & 1) == 0) return false;   /* even numbers >2 are composite */
    for (idx_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

static bool is_power_of_two(idx_t x)
{
    return x != 0 && (x & (x - 1)) == 0;
}

static idx_t next_odd_prime(idx_t n)
{
    idx_t candidate = n + 1;
    if (candidate <= 2) candidate = 3;
    if ((candidate & 1) == 0) candidate++;      /* make odd */
    while (!is_prime(candidate))
        candidate += 2;                         /* skip even numbers */
    return candidate;
}

static idx_t floor_log2(idx_t x)
{
    idx_t exp = 0;
    while (x > 1) {
        x >>= 1;
        exp++;
    }
    return exp;
}

/* ---------- Sequence function ------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if (n == 2) return 1;

    /* Power of two case (n >= 4) */
    if (is_power_of_two(n)) {
        idx_t prime = next_odd_prime(n);
        return ((seq_t)(n / 2)) * (seq_t)prime;
    }

    /* Prime case */
    if (is_prime(n)) {
        idx_t t = n - 1;
        idx_t exp = floor_log2(t);
        idx_t pow = 1ULL << exp;
        return ((seq_t)pow) * (seq_t)n;
    }

    /* All other n produce 0 */
    return 0;
}
