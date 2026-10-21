/*
 * A288636  Height of power-tower factorization of n. Row lengths of A278028.
 * https://oeis.org/A288636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; for n > 1, a(n) = 1 + max{ a(e) | n = b^e with b > 1,
 *            e > 1 }. If n is not an exact power, a(n) = 1
 * Range:     u8 n <= 9779, u16 n <= 9779, u32 n <= 9779, u64 n <= 9779,
 *            u128 n <= 9779, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiply base^exp, stopping if intermediate value would exceed limit. */
static idx_t pow_limit(idx_t base, idx_t exp, idx_t limit)
{
    idx_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
    {
        if (base == 0) return 0;
        if (limit / base < result)   /* would overflow or exceed limit */
            return limit + 1;
        result *= base;
    }
    return result;
}

/* Integer e‑th root of n: returns r if r^e == n, otherwise 0. */
static idx_t int_root(idx_t n, idx_t e)
{
    if (e == 1) return n;
    if (n < 2)   return 0;

    /* Find an upper bound for the root using doubling. */
    idx_t low = 2;
    idx_t high = 2;
    while (pow_limit(high, e, n) <= n)
    {
        high <<= 1;
        if (high > n)
        {
            high = n;
            break;
        }
    }

    /* Binary search between low and high. */
    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        idx_t val = pow_limit(mid, e, n);
        if (val == n) return mid;
        if (val < n)  low = mid + 1;
        else          high = mid - 1;
    }
    return 0;
}

/* Recursive computation of a(n). */
static seq_t compute_height(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    seq_t best = (seq_t)1;  /* default if n is not a perfect power */

    /* Iterate over all possible exponents e ≥ 2 with 2^e ≤ n. */
    for (idx_t e = 2; ; ++e)
    {
        if (pow_limit(2, e, n) > n) break;   /* 2^e would exceed n */

        idx_t root = int_root(n, e);
        if (root >= 2 && pow_limit(root, e, n) == n)
        {
            seq_t sub = compute_height(e);
            seq_t candidate = sub + (seq_t)1;
            if (candidate > best) best = candidate;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    /* This function is deliberately free of static storage; it
       recomputes the value on each call but remains fast for n ≤ 10000. */
    return compute_height(n);
}
