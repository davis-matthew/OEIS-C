/*
 * A290695  Triangle read by rows, denominators of coefficients (in rising
 *          powers) of rational polynomials P(n, x) such that
 *          Integral_{x=0..1} P'(n, x) = Bernoulli(n, 1).
 * https://oeis.org/A290695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, frac
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A290695 – Triangle read by rows, denominators of coefficients
 * of polynomials P(n,x).  The function computes the flattened triangle
 * (row by row).  For an index n the value a(n) is:
 *
 *   Let r be the unique row such that  
 *       (r+1)(r+4)/2 > n   and   r(r+3)/2 ≤ n,
 *   let k = n - r(r+3)/2  (0 ≤ k ≤ r+1).
 *   If k == 0 return 1.
 *   Else the answer is
 *       a(n) = k / gcd( (k-1)! · S2(r,k-1) , k ),
 *   where S2 is the Stirling number of the second kind.
 *
 * The implementation precomputes all necessary Stirling numbers
 * modulo small moduli (≤ 202) and the gcd((k-1)!,k) values.
 * It is fully compliant with C11 and C++17 and uses only integer
 * arithmetic allowed by the problem statement.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Constants: maximum row and modulus needed for n ≤ 10000 */
#define MAX_K 202   /* maximum value of k = r+1 (≤ 201) */
#define MAX_R 201   /* maximum row index needed for n ≤ 10000 */

/* pre‑computed gcd((k-1)!,k) values */
static uint16_t d_gcd_fact[MAX_K + 1];

/* pre‑computed Stirling numbers of the second kind modulo m:
 * S2_mod[m][n][k] = S2(n,k) mod m, for 0 ≤ n,k ≤ MAX_R. */
static uint16_t S2_mod[MAX_K + 1][MAX_R + 1][MAX_R + 1];

/* gcd for idx_t (works in C and C++) */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ------------------------------------------------------------------ */
/* Pre‑compute auxiliary tables                                    */
/* ------------------------------------------------------------------ */
static void precompute_auxiliary(void)
{
    /* ---- compute d_gcd_fact[k] = gcd((k-1)!, k) ---- */
    for (int k = 1; k <= MAX_K; ++k)
    {
        int d = 1;
        int tmp = k;
        for (int p = 2; p * p <= tmp; ++p)
        {
            if (tmp % p == 0)
            {
                int e = 0;
                while (tmp % p == 0)
                {
                    tmp /= p;
                    ++e;
                }
                /* exponent of p in (k-1)! */
                int f = 0;
                int pp = p;
                while (pp <= k - 1)
                {
                    f += (k - 1) / pp;
                    pp *= p;
                }
                int e_min = e < f ? e : f;
                for (int i = 0; i < e_min; ++i)
                    d *= p;
            }
        }
        if (tmp > 1) /* remaining prime factor */
        {
            int p = tmp;
            /* exponent of p in (k-1)! */
            int f = 0;
            int pp = p;
            while (pp <= k - 1)
            {
                f += (k - 1) / pp;
                pp *= p;
            }
            int e_min = 1 < f ? 1 : f;
            for (int i = 0; i < e_min; ++i)
                d *= p;
        }
        d_gcd_fact[k] = (uint16_t)d; /* d ≤ k ≤ 202 */
    }

    /* ---- compute Stirling numbers modulo all moduli ---- */
    for (int m = 1; m <= MAX_K; ++m)
    {
        for (int n = 0; n <= MAX_R; ++n)
        {
            for (int k = 0; k <= n; ++k)
            {
                if (n == 0 && k == 0)
                {
                    S2_mod[m][0][0] = (uint16_t)(1 % m);
                }
                else if (n == 0 || k == 0)
                {
                    S2_mod[m][n][k] = 0;
                }
                else
                {
                    uint16_t v1 = S2_mod[m][n - 1][k - 1];
                    uint16_t v2 = S2_mod[m][n - 1][k];
                    uint16_t val = (v1 + (uint32_t)k * v2 % m) % m;
                    S2_mod[m][n][k] = val;
                }
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* One‑time initialization */
    static bool initialized = false;
    if (!initialized)
    {
        precompute_auxiliary();
        initialized = true;
    }

    /* Find the row r such that M(r-1) ≤ n < M(r) where M(r) = (r+1)(r+4)/2 */
    idx_t low = 0, high = MAX_R;
    while (low < high)
    {
        idx_t mid = (low + high) / 2;
        unsigned long long M = (unsigned long long)(mid + 1) * (mid + 4) / 2;
        if (M > n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                       /* current row index (0‑based) */
    idx_t M_prev = r * (r + 3) / 2;       /* cumulative entries up to row r‑1 */
    idx_t k = n - M_prev;                /* column index within row (0‑based) */

    /* Special case: first coefficient of the row is always 1 */
    if (k == 0)
        return (seq_t)1;

    /* ----- compute the denominator ----- */
    uint16_t d_val = d_gcd_fact[(int)k];      /* gcd((k−1)!, k) */
    idx_t m = k / d_val;                      /* modulus to use for Stirling */
    /* S2(n, k−1) modulo m */
    uint16_t s = S2_mod[m][r][k - 1];
    idx_t g = gcd_idx((idx_t)s, m);           /* gcd(S2(n,k−1), m) */
    seq_t denom = (seq_t)k / ((seq_t)d_val * (seq_t)g);
    return denom;
}
