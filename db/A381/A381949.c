/*
 * A381949  a(n) is the smallest integer k greater than 1 and not a perfect
 *          power satisfying A373387(k^n) = n.
 * https://oeis.org/A381949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, hard, nonn
 * Formula:   a(n) = least k>1, not a perfect power, with A373387(k^n) = n.
 *            Computed by search: the smallest k divisible by 5 comes from the
 *            2-adic condition k = 2^E*t +- 1 (t odd), with E = n for odd n
 *            and E = n - v2(n) for even n. For n <= 26 a 5-adic Hensel-lift
 *            search is also run for k not divisible by 5 b...
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

typedef unsigned long long A381949_ull;
typedef unsigned __int128 A381949_u128;

static A381949_ull A381949_B;
static A381949_ull A381949_best;
static idx_t A381949_n;

static A381949_ull A381949_pm(A381949_ull b, A381949_ull e, A381949_ull M)
{
    A381949_ull r = 1 % M;
    b %= M;
    while (e)
    {
        if (e & 1) r = (A381949_ull)((A381949_u128)r * b % M);
        b = (A381949_ull)((A381949_u128)b * b % M);
        e >>= 1;
    }
    return r;
}

static int A381949_cmppow(seq_t mid, int e, seq_t k)
{
    seq_t res = 1;
    for (int i = 0; i < e; i++)
    {
        if (res > k / mid) return 1;
        res *= mid;
    }
    if (res < k) return -1;
    if (res == k) return 0;
    return 1;
}

static bool A381949_ispp(seq_t k)
{
    seq_t pw2 = 4;
    for (int e = 2; e <= 130 && pw2 != 0 && pw2 <= k; e++, pw2 *= 2)
    {
        seq_t lo = 2, hi = k;
        while (lo <= hi)
        {
            seq_t mid = lo + (hi - lo) / 2;
            int c = A381949_cmppow(mid, e, k);
            if (c == 0) return true;
            if (c < 0) lo = mid + 1;
            else hi = mid - 1;
        }
    }
    return false;
}

/* x such that v5(x) is the valuation in question, computed mod M */
static A381949_ull A381949_x(A381949_ull m, A381949_ull M)
{
    A381949_ull c = m % 5;
    if (c == 1) return (m + M - 1) % M;
    if (c == 4) return (m + 1) % M;
    return (A381949_ull)(((A381949_u128)m * m + 1) % M);
}

static int A381949_P(A381949_ull r, idx_t n, A381949_ull Mj)
{
    A381949_ull m = A381949_pm(r, (A381949_ull)n, Mj);
    if (m % 5 == 0) return 0;
    return A381949_x(m, Mj) == 0;
}

static int A381949_exact(A381949_ull k, idx_t n)
{
    A381949_ull M1 = 1;
    for (idx_t i = 0; i <= n; i++) M1 *= 5;
    A381949_ull m = A381949_pm(k, (A381949_ull)n, M1);
    A381949_ull c = m % 5;
    if (c == 0) return 0;
    A381949_ull x = A381949_x(m, M1);
    idx_t v = 0;
    if (x == 0) v = n + 1;
    else while (x % 5 == 0) { v++; x /= 5; }
    if (c == 4 && v < 1) v = 1;
    return v == n;
}

static void A381949_dfs(A381949_ull r, idx_t j, A381949_ull Mj)
{
    if (r >= A381949_B) return;
    idx_t n = A381949_n;
    if (j == n)
    {
        for (A381949_ull k = r; k < A381949_B; k += Mj)
        {
            if (k > 1 && A381949_exact(k, n) && !A381949_ispp((seq_t)k))
            {
                if (k < A381949_best) A381949_best = k;
                break;
            }
        }
        return;
    }
    for (A381949_ull d = 0; d < 5; d++)
    {
        A381949_ull r2 = r + d * Mj;
        A381949_ull M2 = Mj * 5;
        if (r2 < A381949_B && A381949_P(r2, n, M2))
            A381949_dfs(r2, j + 1, M2);
    }
}

static seq_t A381949_best5(idx_t n)
{
    idx_t E = n;
    if ((n & 1u) == 0u)
    {
        idx_t v = 0, t = n;
        while ((t & 1u) == 0u) { v++; t >>= 1; }
        E = n - v;
    }
    if (E < 2) return 0;
    seq_t two = 1;
    for (idx_t i = 0; i < E; i++) two *= 2;
    for (seq_t t = 1;; t += 2)
    {
        seq_t base = two * t;
        seq_t k = base - 1;
        if (k > 1 && k % 5 == 0 && !A381949_ispp(k)) return k;
        k = base + 1;
        if (k % 5 == 0 && !A381949_ispp(k)) return k;
    }
}

seq_t a(idx_t n)
{
    if (n <= 2)
    {
        for (A381949_ull k = 2;; k++)
        {
            if (k % 5 == 0) continue;
            if (A381949_exact(k, n) && !A381949_ispp((seq_t)k))
                return (seq_t)k;
        }
    }
    seq_t b5 = A381949_best5(n);
    if (n > 26) return b5;

    A381949_n = n;
    A381949_B = (A381949_ull)b5;
    A381949_best = A381949_B;
    for (A381949_ull r = 1; r < 5; r++)
        if (A381949_P(r, n, 5))
            A381949_dfs(r, 1, 5);
    if (A381949_best < A381949_B) return (seq_t)A381949_best;
    return b5;
}
