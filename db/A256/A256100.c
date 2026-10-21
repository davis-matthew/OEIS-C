/*
 * A256100  In S = A007376 (read as a sequence) the digit S(n) appears a(n)
 *          times in the sequence S(1), ..., S(n).
 * https://oeis.org/A256100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 2144, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of times the digit S(n) occurs among S(1), …, S(n),
   where S is the infinite digit sequence 1,2,3,4,… (concatenated) */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static data for memoization and sequence generation */
    static seq_t *cache = NULL;          /* cached a(k) values */
    static size_t  cap   = 0;            /* cache capacity */
    static idx_t   generated = 0;        /* highest n already produced */

    static idx_t curr_num      = 1;      /* current natural number */
    static idx_t pos_in_num    = 0;      /* next digit position in curr_num */
    static idx_t num_len       = 1;      /* number of digits in curr_num */
    static int   num_digits[20];         /* digits of curr_num in order */
    static seq_t counts[10] = {0};       /* cumulative counts of digits 0‑9 */
    static int   init_flag = 0;          /* has the digit buffer been set up? */

    /* Initialisation on first entry */
    if (!init_flag) {
        /* digits of number 1 */
        num_digits[0] = 1;
        counts[0] = 0; counts[1] = 0; counts[2] = 0; counts[3] = 0; counts[4] = 0;
        counts[5] = 0; counts[6] = 0; counts[7] = 0; counts[8] = 0; counts[9] = 0;
        init_flag = 1;
    }

    /* Return pre‑computed value if possible */
    if (n <= generated)
        return cache[(size_t)(n - 1)];

    /* Ensure cache can hold up to n values */
    if (!cache) {
        cap = 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
    } else if ((idx_t)cap < n) {
        size_t newcap = cap ? cap : 8;
        while ((idx_t)newcap < n)
            newcap *= 2;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* Generate values from generated+1 up to n */
    for (idx_t i = generated + 1; i <= n; ++i) {
        /* Move to next natural number when the current one is exhausted */
        if (pos_in_num == num_len) {
            ++curr_num;

            /* Decompose curr_num into decimal digits */
            idx_t temp = curr_num;
            int rev[20];
            int revlen = 0;
            while (temp > 0) {
                rev[revlen++] = (int)(temp % 10);
                temp /= 10;
            }
            num_len = revlen;
            for (int j = 0; j < revlen; ++j)
                num_digits[j] = rev[revlen - 1 - j];
            pos_in_num = 0;
        }

        /* Emit the next digit of S and update counts */
        int digit = num_digits[pos_in_num++];
        counts[digit] += (seq_t)1;

        /* Store a(i) */
        cache[(size_t)(i - 1)] = counts[digit];
    }

    generated = n;
    return cache[(size_t)(n - 1)];
}
