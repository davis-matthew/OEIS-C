/*
 * A383134  Array read by ascending antidiagonals: A(n,k) is the length of the
 *          arithmetic progression of only primes having difference n and
 *          first term prime(k).
 * https://oeis.org/A383134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is found by interpreting n as an entry
 *            (difference,prime_index) * of the array A(d,k) read by ascending
 *            antidiagonals, then counting the * length of the prime
 *            arithmetic progression starting at the k‑th prime * with common
 *            difference d
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_small.h"

/* Generate the first few primes on demand; we need only the first ~200. */
static uint64_t primes[2048];
static size_t prime_cnt = 0;

static void generate_primes_up_to(size_t needed)
{
    if (prime_cnt >= needed)
        return;
    uint64_t cand = (prime_cnt == 0 ? 2 : primes[prime_cnt - 1] + 1);
    while (prime_cnt < needed) {
        if (is_prime_small((seq_t)cand))
            primes[prime_cnt++] = cand;
        cand++;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find the diagonal (sum of indices) that contains position n */
    idx_t s = 1;
    while ((s - 1) * s / 2 < n)
        s++;

    /* Index of the first element on this diagonal (1‑based) */
    idx_t prev = (s - 2) * (s - 1) / 2;      /* 0 when s == 2          */
    idx_t off = n - prev;                    /* Position on the diagonal, 1 ≤ off ≤ s-1 */

    /* Decode (difference d, prime index k) from the antidiagonal order
     * where n decreases from the start of the diagonal. */
    idx_t d = s - off;   /* common difference of the progression */
    idx_t k = off;       /* index (1‑based) of the starting prime   */

    /* Ensure we have enough primes precomputed */
    generate_primes_up_to(k);
    uint64_t start = primes[k - 1];

    /* Count how many consecutive terms of the progression are prime. */
    idx_t len = 0;
    uint64_t cand = start;
    while (is_prime_small((seq_t)cand)) {
        len++;
        cand += d;
    }

    return (seq_t)len;
}
