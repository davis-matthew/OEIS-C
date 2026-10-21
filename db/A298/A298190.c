/*
 * A298190  Number of nX3 0..1 arrays with every element equal to 0, 1, 2, 3
 *          or 5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) -4*a(n-2) -2*a(n-3) +9*a(n-4) -26*a(n-5)
 *            +37*a(n-6) * -19*a(n-7) +11*a(n-8) -62*a(n-9) -6*a(n-10)
 *            +50*a(n-11) +38*a(n-12) * -20*a(n-13) -43*a(n-14) -2*a(n-15)
 *            +20*a(n-16) +8*a(n-17) (n>19) * Initial values: * a(1)=4,
 *            a(2)=26, a(3)=92, a(4)=354, a(5)=1385, a(6)=5450, a(7)=21362,
 *            *...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Direct initial values for n <= 19 */
    if (n <= 19)
    {
        static const seq_t init[20] = {
            0ULL, /* unused */
            4ULL,
            26ULL,
            92ULL,
            354ULL,
            1385ULL,
            5450ULL,
            21362ULL,
            83805ULL,
            328854ULL,
            1290664ULL,
            5064904ULL,
            19876664ULL,
            78004135ULL,
            306122212ULL,
            1201352626ULL,
            4714619350ULL,
            18502172075ULL,
            72610411677ULL,
            284954178325ULL
        };
        return init[(size_t)n];
    }

    /* Compute up to n using the recurrence */
    size_t len = (size_t)n + 1;          /* 1‑based indexing */
    seq_t *buf = (seq_t *)malloc(len * sizeof(seq_t));
    if (!buf) return 0;                  /* allocation failure -> 0 (unlikely) */

    /* Initialize first 19 terms */
    static const seq_t init[20] = {
        0ULL,
        4ULL,
        26ULL,
        92ULL,
        354ULL,
        1385ULL,
        5450ULL,
        21362ULL,
        83805ULL,
        328854ULL,
        1290664ULL,
        5064904ULL,
        19876664ULL,
        78004135ULL,
        306122212ULL,
        1201352626ULL,
        4714619350ULL,
        18502172075ULL,
        72610411677ULL,
        284954178325ULL
    };
    for (idx_t i = 1; i <= 19; ++i)
        buf[i] = init[(size_t)i];

    /* Apply recurrence for i = 20 .. n */
    for (idx_t i = 20; i <= n; ++i)
    {
        seq_t val = (seq_t)5 * buf[i - 1];
        val -= (seq_t)4 * buf[i - 2];
        val -= (seq_t)2 * buf[i - 3];
        val += (seq_t)9 * buf[i - 4];
        val -= (seq_t)26 * buf[i - 5];
        val += (seq_t)37 * buf[i - 6];
        val -= (seq_t)19 * buf[i - 7];
        val += (seq_t)11 * buf[i - 8];
        val -= (seq_t)62 * buf[i - 9];
        val -= (seq_t)6 * buf[i - 10];
        val += (seq_t)50 * buf[i - 11];
        val += (seq_t)38 * buf[i - 12];
        val -= (seq_t)20 * buf[i - 13];
        val -= (seq_t)43 * buf[i - 14];
        val -= (seq_t)2 * buf[i - 15];
        val += (seq_t)20 * buf[i - 16];
        val += (seq_t)8 * buf[i - 17];
        buf[i] = val;
    }

    seq_t result = buf[(size_t)n];
    free(buf);
    return result;
}
