/*
 * A357248  Number of n-node tournaments that have exactly four circular
 *          triads.
 * https://oeis.org/A357248
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * ((7/3)*(n-4) + 4*(n-5) + (7/6)*(n-6)*(n-7)[n>5] +
 *            (1/18)*(n-7)*(n-8)*(n-9)[n>6] + (1/1944)*(n-8)!/(n-12)! [n>11]
 * Range:     u8 none, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence starts at index 5.  For n < 5 the function returns 0. */
seq_t a(idx_t n)
{
    /* Fast factorial with memoisation. */
    static seq_t fact_val = 1;          /* holds fact(fact_max) */
    static idx_t  fact_max = 0;          /* current maximum n computed */

    seq_t fact;

    if (n >= 5) {
        /* Compute or update factorial n! */
        if (n > fact_max) {
            if (fact_max == 0) {
                fact_val = (seq_t)1;   /* 0! = 1 */
                fact_max = 1;          /* now fact_val is 1! */
            }
            for (idx_t i = fact_max + 1; i <= n; ++i) {
                fact_val = fact_val * (seq_t)i;
            }
            fact_max = n;
            fact = fact_val;
        } else if (n < fact_max) {
            /* Recompute factorial directly (rare for 64-bit). */
            seq_t tmp = (seq_t)1;
            for (idx_t i = 2; i <= n; ++i) {
                tmp = tmp * (seq_t)i;
            }
            fact = tmp;
        } else { /* n == fact_max */
            fact = fact_val;
        }

        seq_t res = 0, term = 0;

        /* (7/3)*(n-4) */
        term = fact / 3;                /* fact is divisible by 3 for n>=3 */
        term *= (seq_t)7;
        term *= (seq_t)(n - 4);
        res += term;

        /* 4*(n-5) */
        term = fact;
        term *= (seq_t)4;
        term *= (seq_t)(n - 5);
        res += term;

        if (n > 5) {
            /* (7/6)*(n-6)*(n-7) */
            term = fact / 6;            /* divisible by 6 for n>=3 */
            term *= (seq_t)7;
            term *= (seq_t)(n - 6);
            term *= (seq_t)(n - 7);
            res += term;
        }

        if (n > 6) {
            /* (1/18)*(n-7)*(n-8)*(n-9) */
            term = fact / 18;           /* divisible by 18 for n>=6 */
            term *= (seq_t)(n - 7);
            term *= (seq_t)(n - 8);
            term *= (seq_t)(n - 9);
            res += term;
        }

        if (n > 11) {
            /* (1/1944)*(n-8)!/(n-12)!  = (1/1944)*(n-8)*(n-9)*(n-10)*(n-11) */
            term = fact / 1944;         /* fact divisible by 1944 for n>=12 */
            term *= (seq_t)(n - 8);
            term *= (seq_t)(n - 9);
            term *= (seq_t)(n - 10);
            term *= (seq_t)(n - 11);
            res += term;
        }

        return res;
    } else {
        /* For indices below the sequence offset, the value is unspecified. */
        return (seq_t)0;
    }
}
