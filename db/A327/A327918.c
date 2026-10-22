/*
 * A327918  The 16 pure prime dates of each non-leap year of the form
 *          concatenate(month,day) with month and day also prime numbers.
 * https://oeis.org/A327918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = nth pure prime date of a non‑leap year. Computed by
 *            brute‑force search over prime months and prime days and
 *            selecting those concatenations that are prime. The sequence is
 *            periodic with period 16
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   concat_parts, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/concat_parts.h"

static const seq_t *init_table(void)
{
    static bool initialized = false;
    static seq_t table[16];

    if (!initialized) {
        const idx_t months[5]       = {2, 3, 5, 7, 11};
        const idx_t prime_days[11]  = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31};
        const idx_t days_in_month[13] = {0,
                                         31, 28, 31, 30, 31, 30, 31, 31, 30,
                                         31, 30, 31};

        seq_t buffer[16];
        size_t pos = 0;

        for (size_t mi = 0; mi < 5; ++mi) {
            idx_t m      = months[mi];
            idx_t max_day = days_in_month[(int)m];
            for (size_t di = 0; di < 11; ++di) {
                idx_t d = prime_days[di];
                if (d > max_day) break;
                seq_t val = concat_parts(m, d);
                if (is_prime_big(val)) {
                    buffer[pos++] = val;
                }
            }
        }
        for (size_t i = 0; i < pos; ++i) {
            table[i] = buffer[i];
        }
        initialized = true;
    }
    return table;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset but harmless */
    const seq_t *tbl = init_table();
    size_t idx = (size_t)((n - 1) % 16);
    return tbl[idx];
}
