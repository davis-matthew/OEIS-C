/*
 * A353512  n multiplied by the least nonzero digit missing from its primorial
 *          base representation.
 * https://oeis.org/A353512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n * A329028(n) (A329028 is the least non‑zero digit
 *            missing from the primorial‑base representation of n)
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Local helper: trial division primality test for unsigned integers. */
static inline bool is_prime_idx(idx_t x)
{
    if (x < 2) return false;
    if (x == 2) return true;
    if ((x & 1U) == 0) return false;
    for (idx_t d = 3; d * d <= x; d += 2)
        if (x % d == 0)
            return false;
    return true;
}

seq_t a(idx_t n)
{
    /* Zero has no primorial representation. */
    if (n == 0)
        return 0;

    /* Track which non‑zero digits 1…9 appear in the representation. */
    bool used[10] = {false};

    /* Build primorial weights up to the largest ≤ n.
       weights[0] = 1 (least significant). */
    const size_t MAX_WEIGHTS = 64;   /* plenty for any n in the tests */
    idx_t weights[MAX_WEIGHTS];
    size_t wcnt = 0;

    /* weight 0 : 1 */
    weights[wcnt++] = 1;
    idx_t prod = 1;

    /* Generate successive primorials (product of first k primes). */
    idx_t cand = 2;
    while (1) {
        /* find next prime ≥ cand */
        while (!is_prime_idx(cand))
            cand++;
        /* would the next primorial exceed n? */
        if (prod > n / cand)
            break;
        prod *= cand;
        if (wcnt < MAX_WEIGHTS)
            weights[wcnt++] = prod;
        else
            break;                /* safety; should never hit */
        cand++;                  /* next candidate */
    }

    /* Greedy decomposition from most significant weight downwards. */
    idx_t rem = n;
    for (size_t i = wcnt; i > 0; --i) {
        idx_t weight = weights[i - 1];
        idx_t digit = rem / weight;
        rem -= digit * weight;
        if (digit > 0 && digit <= 9)
            used[(int)digit] = true;
    }

    /* Find least non‑zero digit missing from 1…9. */
    int missing = 1;
    while (missing <= 9 && used[missing])
        missing++;
    if (missing > 9)            /* if all 1‑9 present, next missing is 10 */
        missing = 10;

    /* Multiply n by the missing digit. */
    idx_t prod_val = n * (idx_t)missing;
    return (seq_t)prod_val;
}
