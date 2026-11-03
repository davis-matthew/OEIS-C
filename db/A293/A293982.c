/*
 * A293982  Length (= size) of the orbit of n under iterations of A293975: x
 *          -> x/2 if even, x + nextprime(x) if odd; or -1 if the orbit is
 *          infinite.
 * https://oeis.org/A293982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = size of the orbit of n under * f(x) = x/2 if x is even *
 *            f(x) = x + nextprime(x) if x is odd * The function returns the
 *            number of distinct values in the * orbit of the given index n
 * Range:     u8 n <= 283, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 89 terms through n = 1000 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Return the next prime strictly greater than x.
   Works for arbitrary-precision seq_t values,
   using the provided is_prime_big predicate. */
static inline seq_t next_prime_seq(seq_t x)
{
    seq_t cand = x + 1;
    if (cand <= 1)
        cand = 2;
    else if (cand == 2)
        return 2;
    /* Ensure cand is odd (except for 2) */
    if ((cand & 1) == 0)
        cand++;
    while (!is_prime_big(cand)) {
        cand += 2;
    }
    return cand;
}

/* Function f used in the orbit. */
static inline seq_t f(seq_t x)
{
    if ((x & 1) == 0)          /* even */
        return x / 2;
    else                       /* odd */
        return x + next_prime_seq(x);
}

/* Compute a(n). */
seq_t a(idx_t n)
{
    seq_t start = (seq_t)n;

    /* --- Floyd’s cycle‑finding algorithm --- */
    seq_t slow = f(start);
    seq_t fast = f(f(start));

    while (slow != fast) {
        slow = f(slow);
        fast = f(f(fast));
    }

    /* Find the first element of the cycle. */
    seq_t entry = start;
    while (entry != fast) {
        entry = f(entry);
        fast  = f(fast);
    }

    /* Length of the cycle. */
    seq_t cycle_len = 1;
    seq_t cur = f(entry);
    while (cur != entry) {
        cycle_len++;
        cur = f(cur);
    }

    /* Length of the pre‑cycle (tail). */
    seq_t tail_len = 0;
    cur = start;
    while (cur != entry) {
        tail_len++;
        cur = f(cur);
    }

    return tail_len + cycle_len;
}
