/*
 * A390279  Triangle read by rows: T(n, k) = Sum_{d | gcd(n, k)}
 *          d*Möbius(n/d).
 * https://oeis.org/A390279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 8645, i16 n <= 9182, i32 n <= 9182, i64 n <= 9182,
 *            i128 n <= 9182, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A390279
 *
 *  The array a(n) is obtained by flattening the triangle
 *  T(n,k) defined by
 *
 *      T(n,k) = Σ_{d | gcd(n,k)} d * μ(n/d)
 *
 *  where μ is the Möbius function.
 *
 *  A(0) = 0, A(1) = 1, A(2) = 1, …  are the entries
 *  of this triangle read row–wise.
 *
 *  Implementation details:
 *
 *  * For a given n we find the row number i and the
 *    column index j:
 *
 *        i = max{ m : m(m+1)/2 ≤ n }
 *        j = n - i(i+1)/2          (0 ≤ j ≤ i)
 *
 *    Then a(n) = T(i,j).
 *
 *  * μ is pre‑computed up to 20000 (more than enough
 *    for all i that can occur when n ≤ 10000).
 */

#include <stdint.h>

static int mu[20001] = {0};
static int mu_initialized = 0;

/* --- compute Möbius function up to 20000 ---------------- */
static void init_mu(void)
{
    int limit = 20000;
    mu[0] = 0;
    mu[1] = 1;
    for (int i = 2; i <= limit; ++i)
    {
        int n = i;
        int cnt = 0;
        int sqfree = 1;               /* 1 = true, 0 = false */
        for (int p = 2; (long long)p * p <= n; ++p)
        {
            if (n % p == 0)
            {
                int exp = 0;
                while (n % p == 0)
                {
                    n /= p;
                    ++exp;
                }
                if (exp > 1)
                {
                    sqfree = 0;
                    break;
                }
                ++cnt;
            }
        }
        if (!sqfree)
        {
            mu[i] = 0;
            continue;
        }
        if (n > 1)            /* remaining prime factor */
            ++cnt;
        mu[i] = (cnt & 1) ? -1 : 1;
    }
    mu_initialized = 1;
}

/* --- Euclidean gcd for idx_t ----------------------------- */
static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* --- compute T(n,k) ------------------------------------- */
static seq_t triangle_T(idx_t n, idx_t k)
{
    if (n == 0 && k == 0)   /* special case – defined as 0 */
        return 0;

    idx_t g = gcd_idx(n, k);
    seq_t res = 0;

    for (idx_t d = 1; d * d <= g; ++d)
    {
        if (g % d != 0)
            continue;

        idx_t d1 = d;
        idx_t d2 = g / d;

        /* contribution from divisor d1 */
        idx_t m1 = n / d1;
        res += (seq_t)d1 * (seq_t)mu[m1];

        if (d1 != d2)           /* distinct divisor */
        {
            idx_t m2 = n / d2;
            res += (seq_t)d2 * (seq_t)mu[m2];
        }
    }
    return res;
}

/* ========================================================= */
/*  Public function for OEIS A390279                       */
/* ========================================================= */
seq_t a(idx_t n)
{
    if (!mu_initialized)
        init_mu();

    if (n == 0)
        return 0;

    /* locate the row i (0‑based) that contains index n */
    idx_t i = 0;
    while ((i + 1) * (i + 2) / 2 <= n)
        ++i;

    idx_t start = i * (i + 1) / 2;
    idx_t j = n - start;          /* 0 ≤ j ≤ i */

    return triangle_T(i, j);
}
