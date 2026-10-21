/*
 * A262351  Sum of the parts in the partitions of n into exactly two
 *          squarefree parts.
 * https://oeis.org/A262351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 503, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n * #{ i : 1 ≤ i ≤ ⌊n/2⌋ , both i and n‑i are square‑free }
 * (The Möbius square, μ(i)², is 1 iff i is square‑free, 0 otherwise.)
 *
 * This implementation builds a sieve of square‑free numbers up to the
 * requested n and counts the suitable i values on each call.
 * The function is thread‑safe only if called from a single thread;
 * otherwise the internal static cache is not protected.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Cached square‑free status table. */
static unsigned char *sq = NULL;   /* sq[i] == 1 iff i is square‑free */
static idx_t sq_lim = 0;           /* largest index cached in sq */

/* Forward declarations of helper routines. */
static idx_t isqrt_idx(idx_t x);
static bool is_prime_simple(idx_t p);
static void extend_squarefree(idx_t m);

/* Integer square‑root (monotonic). */
static idx_t isqrt_idx(idx_t x) {
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= x)
        ++r;
    return r;
}

/* Very small prime test: works for p <= √m where m ≈ 10⁴. */
static bool is_prime_simple(idx_t p) {
    if (p < 2) return false;
    for (idx_t d = 2; d * d <= p; ++d)
        if (p % d == 0)
            return false;
    return true;
}

/* Extend the square‑free table to cover all numbers up to m. */
static void extend_squarefree(idx_t m) {
    if (m <= sq_lim)
        return;

    size_t new_size = (size_t)(m + 1);
    unsigned char *new_sq = (unsigned char *)realloc(sq, new_size);
    if (!new_sq)
        abort();  /* allocation failure */

    /* Initialize the new entries as square‑free (1). */
    for (idx_t i = sq_lim + 1; i <= m; ++i)
        new_sq[i] = 1;

    /* Mark multiples of squares of primes as non square‑free. */
    idx_t limit = isqrt_idx(m);
    for (idx_t p = 2; p <= limit; ++p) {
        if (!is_prime_simple(p))
            continue;
        idx_t square = p * p;
        for (idx_t k = square; k <= m; k += square)
            new_sq[k] = 0;
    }

    sq = new_sq;
    sq_lim = m;
}

/* Public sequence routine. */
seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;  /* safety guard – the sequence is defined for n ≥ 1 */

    /* Make the square‑free table large enough. */
    extend_squarefree(n);

    /* Count suitable i. */
    idx_t half = n / 2;
    idx_t count = 0;
    for (idx_t i = 1; i <= half; ++i) {
        if (sq[i] && sq[n - i])
            ++count;
    }

    return (seq_t)n * (seq_t)count;
}
