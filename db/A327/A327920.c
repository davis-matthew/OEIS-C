/*
 * A327920  The 16 pure prime dates of each year of the form
 *          concatenate(day,month) with month and day also prime numbers.
 * https://oeis.org/A327920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The pure prime dates sequence A327920.  
   Each year has the same 16 dates, given by the concatenation
   of a prime day and a prime month (3, 7, 11) where the concatenation
   is also prime.  The sequence is periodic with period 16:
   a(n) = table[(n-1) mod 16] where
   table = {23,53,73,113,173,193,233,293,313,
            37,137,197,317,211,311,2311}.  */

#include <stddef.h>
#include <stdbool.h>

static bool is_prime_small(int n)
{
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1) == 0) return false;
    for (int i = 3; (long long)i * i <= n; i += 2)
        if (n % i == 0) return false;
    return true;
}

static bool is_prime_seq(seq_t n)
{
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1) == 0) return false;
    for (seq_t i = 3; i <= n / i; i += 2)
        if (n % i == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    static seq_t table[16];
    static bool init = false;

    if (!init) {
        int months[3] = {3, 7, 11};
        int idx = 0;

        for (size_t mi = 0; mi < 3; ++mi) {
            int month = months[mi];
            int days = (month == 11) ? 30 : 31;
            seq_t factor = (month < 10) ? 10 : 100;   /* month digits */

            for (int d = 2; d <= days; ++d) {
                if (!is_prime_small(d)) continue;

                seq_t concat = (seq_t)d * factor + (seq_t)month;
                if (is_prime_seq(concat))
                    table[idx++] = concat;
            }
        }
        init = true;
    }

    size_t pos = (size_t)((n - 1) % 16);
    return table[pos];
}
