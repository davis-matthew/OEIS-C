/*
 * A270540  Numbers that are equal to the number of their digits multiplied by
 *          the sum of the fifth powers of the digits.
 * https://oeis.org/A270540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = the n-th integer x such that x = (#digits of x) * sum of
 *            the fifth powers of its digits
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#define MAX_TERMS 10000
#define LIMIT     3000000   /* safe upper bound for any solution */

static const seq_t pow5[10] = {(seq_t)0, (seq_t)1, (seq_t)32,  (seq_t)243, (seq_t)1024,
                               (seq_t)3125, (seq_t)7776, (seq_t)16807, (seq_t)32768, (seq_t)59049};

static seq_t terms[MAX_TERMS];
static size_t terms_len = 0;
static bool terms_built = false;

static void build_terms(void) {
    for (idx_t x = 0; x <= LIMIT; ++x) {
        seq_t sum = 0;
        idx_t tmp = x;
        seq_t d = 0;

        if (tmp == 0) {
            d = 1;
            sum += pow5[0];          /* digit 0 */
        } else {
            while (tmp > 0) {
                unsigned dig = tmp % 10;
                sum += pow5[dig];
                tmp /= 10;
                d++;
            }
        }

        if ((seq_t)x == d * sum) {
            if (terms_len < MAX_TERMS) {
                terms[terms_len++] = (seq_t)x;
            }
        }
    }
    terms_built = true;
}

seq_t a(idx_t n) {
    if (!terms_built) {
        build_terms();
    }

    if (n == 0) {
        return 0;
    }

    size_t idx = (size_t)(n - 1);
    if (idx < terms_len) {
        return terms[idx];
    }

    /* If n is beyond the number of known terms, return 0. */
    return 0;
}
