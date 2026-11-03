/*
 * A344438  a(n) = 1 if n can be written as a product of factorials (A000142),
 *          0 otherwise; Characteristic function of Jordan-Polya numbers
 *          (A001013).
 * https://oeis.org/A344438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n can be written as a product of factorials
 *            (Jordan‑Polya number), 0 otherwise
 * Range:     u8 n <= 65536, u16 n <= 65536, u32 n <= 65536, u64 n <= 65536,
 *            u128 n <= 65536, big any n
 * Validated: standard, pass, 136 terms through n = 8630 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static bool is_jp_rec(idx_t n, const idx_t *facts, size_t count, size_t start)
{
    if (n == 1)
        return true;
    for (size_t i = start; ; --i)
    {
        if (facts[i] <= n && n % facts[i] == 0)
        {
            if (is_jp_rec(n / facts[i], facts, count, i))
                return true;
        }
        if (i == 0)
            break;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* 1 is a trivial Jordan‑Polya number. */
    if (n <= 1)
        return (seq_t)1;

    /* Build factorials ≤ n */
    idx_t facts[24];
    size_t count = 0;
    idx_t f = 1;
    for (idx_t k = 2; ; ++k)
    {
        if (f > n / k)   /* would overflow or exceed n */
            break;
        f *= k;
        facts[count++] = f;
        if (count >= sizeof(facts) / sizeof(facts[0]))
            break;
    }

    if (count == 0)
    {
        /* n < 2 handled above; count==0 cannot happen otherwise */
        return (seq_t)1;
    }

    bool ok = is_jp_rec(n, facts, count, count - 1);
    return (seq_t)(ok ? 1 : 0);
}
