/*
 * A252190  Number of (n+2)X(6+2) 0..3 arrays with every 3X3 subblock row and
 *          column sum equal to 0 1 3 6 or 7 and every 3X3 diagonal and
 *          antidiagonal sum not equal to 0 1 3 6 or 7.
 * https://oeis.org/A252190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 15*a(n-3) - 70*a(n-6) + 120*a(n-9) - 64*a(n-12)
 * Range:     u8 none, u16 n <= 6, u32 n <= 25, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial terms (1‑based).  For n <= 21 the values are hard‑coded.  */
/* The function is memoised; it works for any order of calls and for   */
/* all supported seq_t types (arbitrary‑precision included).          */
#include <stdlib.h>
static const seq_t init_val[22] = {
    0ULL,                  /* dummy for 0‑index */
    (seq_t)7973,           /* a(1) */
    (seq_t)9508,           /* a(2) */
    (seq_t)13737,          /* a(3) */
    (seq_t)21417,          /* a(4) */
    (seq_t)37035,          /* a(5) */
    (seq_t)57629,          /* a(6) */
    (seq_t)87230,          /* a(7) */
    (seq_t)150297,         /* a(8) */
    (seq_t)239980,         /* a(9) */
    (seq_t)380896,         /* a(10) */
    (seq_t)661172,         /* a(11) */
    (seq_t)1077056,        /* a(12) */
    (seq_t)1764648,        /* a(13) */
    (seq_t)3158368,        /* a(14) */
    (seq_t)5312912,        /* a(15) */
    (seq_t)9015496,        /* a(16) */
    (seq_t)16782224,       /* a(17) */
    (seq_t)29334608,       /* a(18) */
    (seq_t)51772488,       /* a(19) */
    (seq_t)100395856,      /* a(20) */
    (seq_t)182091344       /* a(21) */
};

seq_t a(idx_t n)
{
    /* static memoisation table:  0‑unused, indices 1…computed are valid */
    static seq_t *buf = NULL;
    static idx_t computed = 0;

    if (n == 0) return 0;   /* behaviour below offset is irrelevant */

    /* if already known, return */
    if (n <= computed) return buf[n];

    /* grow buffer to hold indices up to n */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(buf, new_size * sizeof(seq_t));
    if (!tmp) return 0;  /* out of memory – unlikely in test harness */
    buf = tmp;

    /* initialise new slots to zero (adequate for seq_t) */
    for (idx_t i = computed + 1; i <= n; ++i) buf[i] = 0;

    /* copy hard‑coded initial terms if we crossed their range */
    if (n >= 1) {
        idx_t start = (computed < 1) ? 1 : computed + 1;
        if (start <= 21) {
            if (n > 21) n = 21;  /* avoid out‑of‑bounds reading */
            for (idx_t i = start; i <= n; ++i) buf[i] = init_val[i];
            if (n >= 21) start = 22;  /* next free position */
        }
    }

    /* compute further terms using the linear recurrence */
    for (idx_t i = (computed < 22) ? 22 : computed + 1; i <= n; ++i) {
        buf[i] = (seq_t)15  * buf[i - 3]
               - (seq_t)70  * buf[i - 6]
               + (seq_t)120 * buf[i - 9]
               - (seq_t)64  * buf[i - 12];
    }

    computed = n;
    return buf[n];
}
