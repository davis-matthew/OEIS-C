/*
 * A349926  Dirichlet g.f.: Product_{k>=2} (1 - 3 * k^(-s)).
 * https://oeis.org/A349926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sum_{S⊆{k≥2}, ∏S=n} (-3)^{|S|}
 * Range:     i8 n <= 70, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements recursive enumeration of distinct factorisations. */

#include <stddef.h>

static seq_t dfs(const idx_t *divs, size_t m, size_t pos, idx_t rem, seq_t coeff)
{
    if (rem == 1)          /* all factors chosen */
        return coeff;
    if (pos == m)          /* no divisors left */
        return 0;

    seq_t total = 0;

    /* Skip current divisor */
    total += dfs(divs, m, pos + 1, rem, coeff);

    /* Take current divisor if it divides remaining product */
    if (rem % divs[pos] == 0) {
        seq_t newcoeff = coeff * (seq_t)(-3);
        idx_t newrem = rem / divs[pos];
        total += dfs(divs, m, pos + 1, newrem, newcoeff);
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    /* Build sorted list of divisors ≥2 */
    idx_t temp = n;
    idx_t divbuf[128];
    size_t dcount = 0;

    /* Small divisors */
    for (idx_t i = 2; i * i <= temp; ++i) {
        if (temp % i == 0) {
            divbuf[dcount++] = i;
            idx_t j = temp / i;
            if (j != i && j != 1)
                divbuf[dcount++] = j;
        }
    }
    /* n itself */
    if (temp != 1)
        divbuf[dcount++] = temp;

    /* Sort divisors ascending (simple insertion sort for tiny array) */
    for (size_t i = 1; i < dcount; ++i) {
        idx_t key = divbuf[i];
        size_t j = i;
        while (j > 0 && divbuf[j - 1] > key) {
            divbuf[j] = divbuf[j - 1];
            --j;
        }
        divbuf[j] = key;
    }

    return dfs(divbuf, dcount, 0, n, 1);
}
