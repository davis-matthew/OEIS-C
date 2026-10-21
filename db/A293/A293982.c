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
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

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
