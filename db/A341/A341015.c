/*
 * A341015  Numbers k such that A124446(k) = 1.
 * https://oeis.org/A341015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth smallest number among {3^j, 2·3^j, 5^j, 4} with
 *            duplicates removed
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* not defined for offset 1 */

    /* static state */
    static seq_t *list = NULL;          /* array of produced terms */
    static size_t list_len = 0;         /* number of terms stored */
    static size_t list_cap = 0;         /* allocated capacity of list */

    static seq_t val3 = 0;              /* next term of 3^j            */
    static seq_t val2 = 0;              /* next term of 2·3^j          */
    static seq_t val5 = 0;              /* next term of 5^j            */
    static bool      inserted4 = false;/* whether 4 has been inserted */
    static bool      initialized   = false;

    /* initialize on first call */
    if (!initialized) {
        val3 = 1;
        val2 = 2;
        val5 = 1;
        inserted4 = false;
        list_cap = 64;
        list = (seq_t*)malloc(list_cap * sizeof(seq_t));
        if (!list) abort();
        list_len = 0;
        initialized = true;
    }

    /* generate terms until we have enough */
    while (list_len < (size_t)n) {
        /* determine current minimum among the three sequences */
        seq_t min12 = (val3 <= val2) ? val3 : val2;
        seq_t min123 = (min12 <= val5) ? min12 : val5;

        /* choose candidate: 4 may appear between 3 and 5 */
        seq_t four = (seq_t)4;
        seq_t candidate;
        if (!inserted4 && four <= min123)
            candidate = four;
        else
            candidate = min123;

        /* append if new */
        if (list_len == 0 || candidate != list[list_len - 1]) {
            if (list_len >= list_cap) {
                list_cap *= 2;
                list = (seq_t*)realloc(list, list_cap * sizeof(seq_t));
                if (!list) abort();
            }
            list[list_len++] = candidate;
        }

        /* advance any sequence that produced this candidate */
        if (candidate == val3) val3 = val3 * (seq_t)3;
        if (candidate == val2) val2 = val2 * (seq_t)3;
        if (candidate == val5) val5 = val5 * (seq_t)5;
        if (!inserted4 && candidate == four) inserted4 = true;
    }

    return list[n - 1];
}
