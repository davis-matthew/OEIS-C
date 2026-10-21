/*
 * A263503  Number of (n+1) X (1+1) arrays of permutations of 0..n*2+1 filled
 *          by rows with each element moved a city block distance of 0 1 or 2,
 *          and rows and columns in increasing lexicographic order.
 * https://oeis.org/A263503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) -2*a(n-2) -12*a(n-3) -18*a(n-4) -138*a(n-5) *
 *            +58*a(n-6) +109*a(n-7) +35*a(n-8) +97*a(n-9) * -36*a(n-10)
 *            -60*a(n-11) +4*a(n-12) -2*a(n-13) +a(n-15) * Initial values
 *            (a(1) … a(15)) given in OEIS
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static const seq_t init_vals[15] = {
    (seq_t)8ULL,          /* a(1) */
    (seq_t)59ULL,         /* a(2) */
    (seq_t)360ULL,        /* a(3) */
    (seq_t)2196ULL,       /* a(4) */
    (seq_t)13704ULL,      /* a(5) */
    (seq_t)85189ULL,      /* a(6) */
    (seq_t)528559ULL,     /* a(7) */
    (seq_t)3280284ULL,    /* a(8) */
    (seq_t)20360536ULL,   /* a(9) */
    (seq_t)126365281ULL,  /* a(10) */
    (seq_t)784254518ULL,  /* a(11) */
    (seq_t)4867282254ULL, /* a(12) */
    (seq_t)30207532735ULL,/* a(13) */
    (seq_t)187474861266ULL,/* a(14) */
    (seq_t)1163511187403ULL/* a(15) */
};

seq_t a(idx_t n)
{
    if (n <= 15)
        return init_vals[n - 1];

    /* ring buffer of the last 15 values */
    seq_t buf[15];
    for (int i = 0; i < 15; ++i)
        buf[i] = init_vals[i];

    /* position of a(i-1) in buf for current i */
    idx_t pos = 14;          /* after initialization pos corresponds to a(15) */

    for (idx_t i = 16; i <= n; ++i) {
        /* indices of a(i-k) in the ring buffer */
        seq_t ai_1  = buf[pos];
        seq_t ai_2  = buf[(pos + 14) % 15];
        seq_t ai_3  = buf[(pos + 13) % 15];
        seq_t ai_4  = buf[(pos + 12) % 15];
        seq_t ai_5  = buf[(pos + 11) % 15];
        seq_t ai_6  = buf[(pos + 10) % 15];
        seq_t ai_7  = buf[(pos + 9) % 15];
        seq_t ai_8  = buf[(pos + 8) % 15];
        seq_t ai_9  = buf[(pos + 7) % 15];
        seq_t ai_10 = buf[(pos + 6) % 15];
        seq_t ai_11 = buf[(pos + 5) % 15];
        seq_t ai_12 = buf[(pos + 4) % 15];
        seq_t ai_13 = buf[(pos + 3) % 15];
        seq_t ai_15 = buf[(pos + 1) % 15];

        /* compute new value using the recurrence */
        seq_t new_val =
              (seq_t)7  * ai_1
            - (seq_t)2  * ai_2
            - (seq_t)12 * ai_3
            - (seq_t)18 * ai_4
            - (seq_t)138 * ai_5
            + (seq_t)58 * ai_6
            + (seq_t)109 * ai_7
            + (seq_t)35 * ai_8
            + (seq_t)97 * ai_9
            - (seq_t)36 * ai_10
            - (seq_t)60 * ai_11
            + (seq_t)4  * ai_12
            - (seq_t)2  * ai_13
            + (seq_t)1  * ai_15;

        /* advance ring buffer */
        pos = (pos + 1) % 15;
        buf[pos] = new_val;
    }

    /* after loop, buf[pos] holds a(n) */
    return buf[pos];
}
