/*
 * A293686  8-digit numbers (padded with leading zeros where necessary) in
 *          which the sum of the number consisting of the first four digits
 *          and the number consisting of the last four digits equals the
 *          number consisting of the middle four digits.
 * https://oeis.org/A293686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, fini, full
 * Formula:   Generates the nth term of OEIS A293686 By enumerating all
 *            integer solutions to the digit condition (first four digits +
 *            last four digits = middle four digits) in ascending order
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static seq_t seq[5050];
static bool seq_initialized = false;

static void generate_seq(void)
{
    size_t pos = 0;

    /* a = 0, first branch (d = 0) */
    seq[pos++] = 0;                     /* b = 0 */

    for (idx_t b = 1; b < 100; ++b) {   /* b = 1 .. 99 */
        seq[pos++] = (seq_t)10100 * b - 1; /* lower value 10100*b - 1 */
        seq[pos++] = (seq_t)10100 * b;    /* higher value 10100*b   */
    }

    /* a = 0, second branch (d = 99) is already included
       in the loop above (lower values). */

    /* a = 1 .. 98 */
    for (idx_t a = 1; a < 99; ++a) {
        for (idx_t b = a + 1; b < 100; ++b) {
            seq[pos++] = (seq_t)999899 * a - 1 + (seq_t)10100 * b;
        }
    }

    /* pos should now be 5050 */
}

seq_t a(idx_t n)
{
    if (!seq_initialized) {
        generate_seq();
        seq_initialized = true;
    }

    /* sequence is 1‑based */
    if (n == 0)
        return 0;                     /* undefined for n == 0, return 0 */

    if (n <= 5050)
        return seq[n - 1];

    return 0;                         /* out of range */
}
