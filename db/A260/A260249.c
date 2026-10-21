/*
 * A260249  Month numbers in the Gregorian calendar, in alphabetical order of
 *          their English names.
 * https://oeis.org/A260249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, word
 * Formula:   a(n) = month number at alphabetical position n (returns 0 for n
 *            > 12)
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the sequence by sorting the month names */
#include <string.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Names of the Gregorian months in order of their numbers */
    const char *month_names[12] = {
        "January", "February", "March", "April",
        "May", "June", "July", "August",
        "September", "October", "November", "December"
    };

    static seq_t order[12] = {0};
    static bool initialized = false;

    if (!initialized) {
        /* Create an array of indices 0..11 */
        int idx[12];
        for (idx_t i = 0; i < 12; ++i)
            idx[i] = (int)i;

        /* Simple insertion sort on the indices using month names */
        for (idx_t i = 1; i < 12; ++i) {
            int j = i;
            while (j > 0 &&
                   strcmp(month_names[idx[j]], month_names[idx[j - 1]]) < 0) {
                int tmp = idx[j];
                idx[j] = idx[j - 1];
                idx[j - 1] = tmp;
                --j;
            }
        }

        /* Fill the sorted month numbers */
        for (idx_t i = 0; i < 12; ++i)
            order[i] = (seq_t)(idx[i] + 1);

        initialized = true;
    }

    if (n < 1)
        return 0;
    if (n > 12)
        return 0;

    return order[n - 1];
}
