/*
 * A350560  Denominators of the fractional, multiplicative Van Eck Sequence
 *          f(n): for n >= 2, if there exists an m < n such that f(m) = f(n),
 *          take the largest such m. If f(n)=1, set f(n+1) = 1/(n-m);
 *          otherwise, if f(n) != 1, set f(n+1) = f(n)+f(m)*(n-m). If no m
 *          exists such that f(m) = f(n), then set f(n+1)=...
 * https://oeis.org/A350560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A350560 – Denominator of the fractional, multiplicative Van Eck Sequence
    a(n) = denominator of f(n) where
      f(1)=1, f(2)=0
      for n≥2:
        let m be the largest index < n with f(m)=f(n);
        if such m exists:
            if f(n)=1      → f(n+1)=1/(n-m);
            else           → f(n+1)=f(n)+f(m)*(n-m);
        else             → f(n+1)=1.
    Initial terms: a(1)=1, a(2)=1, a(3)=1, a(4)=2, …   */
/*  Note: the sequence is computed incrementally and memoized.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t *num = NULL;
static seq_t *den = NULL;
static idx_t  size = 0;
static idx_t  computed = 0;

/* Ensure arrays can hold indices up to n (inclusive).  */
static void ensure_capacity(idx_t n)
{
    if (size < n + 1) {
        idx_t new_size = size ? size * 2 : 16;
        if (new_size < n + 1) new_size = n + 1;
        num = (seq_t*)realloc(num, new_size * sizeof(seq_t));
        den = (seq_t*)realloc(den, new_size * sizeof(seq_t));
        size = new_size;
    }
}

/* Main function – returns denominator of f(n).  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* out of range, shouldn't happen */

    /* initialise base cases on first call */
    if (!num) {
        ensure_capacity(n);
        num[1] = 1;  den[1] = 1;              /* f(1)=1/1  */
        num[2] = 0;  den[2] = 1;              /* f(2)=0/1  */
        computed = 2;
    }

    /* extend computation up to n */
    if (computed < n) {
        ensure_capacity(n);
        for (idx_t i = computed; i < n; ++i)     /* compute f(i+1) */
        {
            /* find largest m<i with f(m)=f(i) */
            idx_t m = 0;
            for (idx_t j = i - 1; j > 0; --j) {
                if (num[j] == num[i] && den[j] == den[i]) {
                    m = j;
                    break;
                }
            }

            if (m == 0) {                           /* no previous occurrence */
                num[i + 1] = 1;
                den[i + 1] = 1;
            } else if (num[i] == 1 && den[i] == 1) { /* f(i)=1  */
                seq_t diff = (seq_t)(i - m);
                num[i + 1] = 1;
                den[i + 1] = diff;
            } else {                                /* general case */
                seq_t diff = (seq_t)(i - m);

                seq_t a_num = num[i];
                seq_t a_den = den[i];
                seq_t b_num = num[m] * diff;
                seq_t b_den = den[m];

                seq_t g = gcd(a_den, b_den);
                seq_t lcm = (a_den / g) * b_den;
                seq_t term1 = a_num * (b_den / g);
                seq_t term2 = b_num * (a_den / g);
                seq_t new_num = term1 + term2;
                seq_t new_den = lcm;

                seq_t h = gcd(new_num, new_den);
                num[i + 1] = new_num / h;
                den[i + 1] = new_den / h;
            }
        }
        computed = n;
    }

    return den[n];
}
