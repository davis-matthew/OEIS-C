/*
 * A339738  Indices of distinct Gregorian calendar year types in a 400-year
 *          period, indexed in order of occurrence in the 3rd millennium CE.
 * https://oeis.org/A339738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 400, u16 n <= 400, u32 n <= 400, u64 n <= 400,
 *            u128 n <= 400, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: assign increasing ids to first-encountered Gregorian year type
   codes over the 400‑year cycle starting at 2001.  The id is returned for
   each year in the 3rd millennium CE. */
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t year_type_index[400];
    static uint8_t code_to_id[14];
    static uint8_t next_id = 0;

    if (!initialized)
    {
        int start_day = 0;           /* Monday for 2001 CE */
        int year = 2001;
        for (idx_t i = 0; i < 400; ++i)
        {
            bool leap = ((year % 400) == 0) || ((year % 4) == 0 && (year % 100) != 0);
            int code = start_day + (leap ? 7 : 0);   /* 0–13 */

            if (code_to_id[code] == 0)
            {
                ++next_id;
                code_to_id[code] = next_id;
            }

            year_type_index[i] = (seq_t)code_to_id[code];
            start_day = (start_day + (leap ? 2 : 1)) % 7;
            ++year;
        }
        initialized = true;
    }

    idx_t pos = (n - 1) % 400;
    return year_type_index[pos];
}
