/*
 * A334823  Triangle, read by rows, of Lambert's denominator polynomials
 *          related to convergents of tan(x).
 * https://oeis.org/A334823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) is the coefficient of x^e in the Lambert denominator
 *            polynomial f(r,x) where r is the row such that r*(r+1)/2 ≤ n <
 *            (r+1)*(r+2)/2 (0‑based rows). Let p = n – r*(r+1)/2 and e = r –
 *            p; if e and r have different parity the coefficient is 0.
 *            Otherwise k = (r‑e)/2 and a(n) = (-1)^k * (2r-2k)! / [...
 * Range:     i8 n <= 14, i16 n <= 27, i32 n <= 65, i64 n <= 170,
 *            i128 n <= 434, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Prime table and helpers ---------------------------------------------------*/
#define PRIME_LIMIT   20000             /* 2*max(r) for r ≤ 10000          */
#define PRIME_MAX_CNT 2263              /* π(20000)+1                       */

static bool primes_ready = false;
static int  primes[PRIME_MAX_CNT];
static int  prime_count = 0;

/* Simple deterministic integer square‑root, 0 ≤ n ≤ 1e9 (fits 64‑bit). */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq  = mid * mid;
        if (sq == n)        return mid;
        if (sq < n)  low  = mid + 1;
        else           high = mid - 1;
    }
    return high;   /* floor sqrt */
}

/* Exponent of prime p in n! (Legendre’s formula) */
static int exp_in_factorial(idx_t n, int p)
{
    int e = 0;
    while (n) {
        n /= p;
        e += (int)n;
    }
    return e;
}

/* Fast integer power of seq_t, exp ≥ 0. */
static seq_t pow_seq_seq(seq_t base, int exp)
{
    seq_t result = 1;
    seq_t cur    = base;
    while (exp) {
        if (exp & 1) result = result * cur;
        cur = cur * cur;
        exp >>= 1;
    }
    return result;
}

/* Initialise static prime table up to PRIME_LIMIT. */
static void init_primes(void)
{
    if (primes_ready) return;
    bool mark[PRIME_LIMIT + 1];
    for (int i = 0; i <= PRIME_LIMIT; ++i) mark[i] = true;
    mark[0] = mark[1] = false;
    for (int i = 2; i * i <= PRIME_LIMIT; ++i)
        if (mark[i])
            for (int j = i * i; j <= PRIME_LIMIT; j += i) mark[j] = false;
    prime_count = 0;
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (mark[i]) primes[prime_count++] = i;
    primes_ready = true;
}

/* Compute the coefficient a(n). -------------------------------------------*/
seq_t a(idx_t n)
{
    init_primes();

    /* Determine row r, start index s, position p and exponent e. */
    idx_t t = isqrt_u64(8 * n + 1);
    idx_t r = (t - 1) / 2;
    while (r * (r + 1) / 2 > n) --r;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    idx_t start  = r * (r + 1) / 2;
    idx_t p      = n - start;           /* 0‑based position in the row */
    idx_t e      = r - p;                /* exponent of x in the term    */
    if (((r - e) & 1u) != 0)            /* parity mismatch -> 0          */
        return 0;

    idx_t k = (r - e) >> 1;              /* k = (r‑e)/2                   */

    /* Compute prime exponents of the coefficient. */
    seq_t coeff = 1;
    for (int i = 0; i < prime_count; ++i) {
        int pval = primes[i];
        if (pval > (int)(2 * r)) break;   /* factorial arguments ≤ 2r */
        int exp = exp_in_factorial(2 * r - 2 * k, pval)
                - exp_in_factorial(2 * k, pval)
                - exp_in_factorial(e,     pval);
        if (pval == 2) exp -= (int)e;
        if (exp > 0)
            coeff = coeff * pow_seq_seq((seq_t)pval, exp);
    }

    /* Apply the sign (-1)^k. */
    if (k & 1)
        coeff = -coeff;

    return coeff;
}
