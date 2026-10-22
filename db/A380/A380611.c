/*
 * A380611  Irregular triangle read by rows: T(r,c) is the product of the
 *          number of standard Young tableaux (A117506) and the number of
 *          semistandard Young tableaux (A262030) for partitions of r.
 * https://oeis.org/A380611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n‑th entry (0‑based) of the * irregular triangle
 *            read by rows. * T(r,λ) = SYT(λ) · SSYT(λ,r) * where λ runs over
 *            all partitions of r. * a(n) is obtained by flattening all rows
 *            in order of increasing r * (each row is listed in reverse
 *            lexicographic order)
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   factorial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factorial.h"
#include "../utils/gcd.h"

/* Compute T(r,λ) for a given partition λ of r.
 * parts[0..len-1] contains the parts of λ, len = number of parts.
 * r is the row index (the maximum entry allowed in the SSYT). */
static seq_t compute_T(idx_t r, const int *parts, size_t len)
{
    seq_t num = factorial(r);   /* numerator starts with r!   */
    seq_t den = 1;              /* denominator starts as 1     */

    /* first pass: multiply by (r + content) and one hook factor */
    for (size_t i = 0; i < len; ++i) {
        for (int j = 0; j < parts[i]; ++j) {
            /* compute hook length of cell (i,j) */
            int below = 0;
            for (size_t k = i + 1; k < len; ++k)
                if (parts[k] > j) ++below;
            int hook = parts[i] - j + below;
            int content = j - (int)i;

            /* multiply by (r + content) */
            seq_t mult = (seq_t)(r + content);
            seq_t g = gcd(num, den);
            num /= g; den /= g;
            num *= mult;
            g = gcd(num, den);
            num /= g; den /= g;

            /* multiply by one hook factor, reducing first */
            seq_t h = (seq_t)hook;
            seq_t g2 = gcd(num, h);
            num /= g2; h /= g2;
            den *= h;
            g = gcd(num, den);
            num /= g; den /= g;
        }
    }

    /* second pass: multiply by the remaining hook factor of each cell */
    for (size_t i = 0; i < len; ++i) {
        for (int j = 0; j < parts[i]; ++j) {
            int below = 0;
            for (size_t k = i + 1; k < len; ++k)
                if (parts[k] > j) ++below;
            int hook = parts[i] - j + below;

            seq_t h = (seq_t)hook;
            seq_t g2 = gcd(num, h);
            num /= g2; h /= g2;
            den *= h;
            seq_t g = gcd(num, den);
            num /= g; den /= g;
        }
    }

    return num / den;   /* den should be 1 */
}

/* Return the n‑th term (0‑based) of the sequence. */
seq_t a(idx_t n)
{
    idx_t cumulative = 0;                    /* running index of terms */
    for (idx_t r = 0; ; ++r) {

        /* handle the special case r = 0 (empty partition) */
        if (r == 0) {
            seq_t val = (seq_t)1;
            if (cumulative == n)
                return val;
            ++cumulative;
            continue;
        }

        /* generate all partitions of r in reverse lexicographic order */
        int *parts = (int*)malloc((size_t)r * sizeof(int));
        if (!parts) return 0;                 /* out of memory */

        parts[0] = (int)r;                    /* first partition: [r] */
        int k = 0;                            /* last index of current partition */

        while (1) {
            size_t len = (size_t)(k + 1);
            seq_t val = compute_T(r, parts, len);

            if (cumulative == n) {
                free(parts);
                return val;
            }
            ++cumulative;

            /* generate next partition of r */
            int rem = 0;
            while (k >= 0 && parts[k] == 1) {
                rem += parts[k];
                --k;
            }
            if (k < 0) {                       /* finished all partitions */
                free(parts);
                break;
            }
            --parts[k];
            ++rem;
            while (rem > parts[k]) {
                parts[k + 1] = parts[k];
                rem -= parts[k];
                ++k;
            }
            parts[k + 1] = rem;
            ++k;
        }
    }
}
