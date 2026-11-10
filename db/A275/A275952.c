/*
 * A275952  a(n) = A225901(A256450(A225901(n))).
 * https://oeis.org/A275952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A225901(A256450(A225901(n))); A256450(m) found by search
 *            (m-th number with a 1 digit in factorial base, a(0)=1)
 * Range:     u8 n <= 23, u16 n <= 5039, u32 n <= 5039, u64 n <= 5039,
 *            u128 n <= 5039, big any n
 * Validated: standard, pass, 102 terms through n = 4969 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* A225901: factorial-base digits d of n at position k (weight k!), nonzero d -> (k+1-d) */
static seq_t A225901_a(seq_t n)
{
    seq_t result = 0;
    seq_t t = n;
    seq_t fac = 1;          /* k! */
    seq_t base = 2;         /* k+1 */
    while (t > 0) {
        seq_t d = t % base;
        t = t / base;
        if (d != 0)
            result += (base - d) * fac;
        fac = fac * base;   /* (k+1)! */
        base++;
    }
    return result;
}

static bool A256450_has_one(seq_t m)
{
    seq_t t = m;
    seq_t base = 2;
    while (t > 0) {
        if (t % base == 1)
            return true;
        t = t / base;
        base++;
    }
    return false;
}

/* A256450: m-th (offset 0) number with a digit 1 in factorial base */
static seq_t A256450_a(seq_t m)
{
    seq_t c = 1;
    seq_t cnt = 0;
    while (cnt < m) {
        c++;
        if (A256450_has_one(c))
            cnt++;
    }
    return c;
}

seq_t a(idx_t n)
{
    seq_t x = A225901_a((seq_t)n);
    seq_t y = A256450_a(x);
    return A225901_a(y);
}
