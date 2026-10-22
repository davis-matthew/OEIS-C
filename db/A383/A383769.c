/*
 * A383769  Denominators of the sequence whose Dirichlet convolution with
 *          itself yields cubes (A000578).
 * https://oeis.org/A383769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of the multiplicative rational sequence c(n)
 *            whose Dirichlet convolution with itself yields n^3 (A000578)
 * Range:     u8 n <= 242, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper structure: fraction num / 2^den. */
typedef struct { __int128 num; unsigned int den; } frac;

/* Compute c(p^k) (fraction) where p is prime and k >= 1. */
static frac get_cp(uint64_t p, uint64_t k)
{
    /* Allocate array for c[0..k] */
    frac *c = (frac *)malloc((k + 1) * sizeof(frac));
    if (!c) return (frac){0, 0};
    c[0].num = 1;
    c[0].den = 0;

    for (uint64_t i = 1; i <= k; ++i) {
        /* sum_{j=1}^{i-1} c_j * c_{i-j} */
        frac sum = {0, 0};
        for (uint64_t j = 1; j < i; ++j) {
            /* product */
            frac prod;
            prod.num = c[j].num * c[i - j].num;
            prod.den = c[j].den + c[i - j].den;

            /* sum += prod */
            if (sum.den == prod.den) {
                sum.num += prod.num;
            } else if (sum.den > prod.den) {
                sum.num += prod.num << (sum.den - prod.den);
            } else {
                sum.num <<= (prod.den - sum.den);
                sum.num += prod.num;
                sum.den = prod.den;
            }
        }

        /* p^(3*i) */
        __int128 pw = 1;
        for (uint64_t t = 0; t < 3 * i; ++t) pw *= (__int128)p;

        /* diff = p^(3*i) - sum */
        __int128 diff = (pw << sum.den) - sum.num;
        unsigned int denom = sum.den + 1;

        /* cancel powers of two */
        while ((diff & 1) == 0 && denom > 0) {
            diff >>= 1;
            denom--;
        }

        c[i].num = diff;
        c[i].den = denom;
    }

    frac result = c[k];
    free(c);
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; this value is unused */

    /* Factorization */
    uint64_t m = (uint64_t)n;
    __int128 total_num = 1;
    unsigned int total_den = 0;

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            frac fp = get_cp(p, exp);
            total_num *= fp.num;
            total_den += fp.den;
            while ((total_num & 1) == 0 && total_den > 0) {
                total_num >>= 1;
                total_den--;
            }
        }
    }
    if (m > 1) {   /* remaining prime factor */
        frac fp = get_cp(m, 1);
        total_num *= fp.num;
        total_den += fp.den;
        while ((total_num & 1) == 0 && total_den > 0) {
            total_num >>= 1;
            total_den--;
        }
    }

    seq_t denom = (seq_t)1;
    denom <<= total_den;
    return denom;
}
