/*
 * A260096  Numbers whose decimal and hexadecimal representations both have
 *          strictly decreasing digits.
 * https://oeis.org/A260096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) is the n-th non‑negative integer whose decimal and
 *            hexadecimal representations both have strictly decreasing
 *            digits. This implementation generates all numbers with
 *            decreasing decimal digits (≤1023 such numbers), checks the
 *            hexadecimal decreasing property, sorts them, and returns the
 *            (n‑1)-th...
 * Range:     u8 n <= 1, u16 n <= 24, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static int initialized = 0;                     /* init flag           */
static seq_t list[1024];                        /* all valid numbers   */
static size_t count;                            /* number of elements */

/* ------------------------------------------------------------------ */
/* Helper: sort list in ascending order (simple insertion sort).      */
/* ------------------------------------------------------------------ */
static void sort_list(void)
{
    for (size_t i = 1; i < count; ++i) {
        seq_t key = list[i];
        int j = (int)i - 1;
        while (j >= 0 && list[j] > key) {
            list[j + 1] = list[j];
            --j;
        }
        list[j + 1] = key;
    }
}

/* ------------------------------------------------------------------ */
/* Helper: initialize the list of all terms.                           */
/* ------------------------------------------------------------------ */
static void init(void)
{
    if (initialized)
        return;

    count = 0;

    /* generate all numbers with strictly decreasing decimal digits.
       There are 2^10 - 1 = 1023 non‑empty subsets of {0..9}. */
    for (int mask = 1; mask < (1 << 10); ++mask) {
        seq_t num = 0;
        /* digits in descending order */
        for (int d = 9; d >= 0; --d) {
            if (mask & (1 << d))
                num = num * 10 + d;
        }

        /* test hex representation for strictly decreasing digits */
        bool hex_ok = true;
        seq_t tmp = num;
        seq_t rev[20];
        size_t len = 0;

        if (tmp == 0) {
            len = 1;
            rev[0] = 0;
        } else {
            while (tmp > 0) {
                rev[len++] = tmp % 16;
                tmp /= 16;
            }
        }

        for (size_t i = 1; i < len; ++i) {
            if (rev[i] <= rev[i - 1]) {
                hex_ok = false;
                break;
            }
        }

        if (hex_ok) {
            list[count++] = num;
        }
    }

    /* sort ascending */
    sort_list();

    initialized = 1;
}

/* ------------------------------------------------------------------ */
/* Public sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init();

    if (n == 0 || n > (idx_t)count)
        return 0;                            /* undefined: return 0 */

    return list[(size_t)n - 1];
}
