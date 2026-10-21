/*
 * A279620  Limit of the sequence of words defined by w(1) = 1, w(2) = 1221,
 *          and w(n) = w(n-1) 2 w(n-2) 2 w(n-1) for n >= 2. Also the fixed
 *          point of the map 1 -> 122, 2 -> 12.
 * https://oeis.org/A279620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 130, u16 n <= 130, u32 n <= 130, u64 n <= 130,
 *            u128 n <= 130, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A279620: limit of words defined by
   w(1)=1, w(2)=1221, w(n)=w(n-1) 2 w(n-2) 2 w(n-1)
   which is the fixed point of the morphism 1→122, 2→12. */
#include <stdint.h>

static idx_t lenX[64];
static idx_t lenY[64];
static int max_level = -1;

/* Pre‑compute expansion lengths until lenX[max_level] >= need. */
static void ensure_lengths(idx_t need)
{
    if (max_level < 0) {
        max_level = 0;
        lenX[0] = 1;
        lenY[0] = 1;
    }
    while (lenX[max_level] < need) {
        idx_t next = max_level + 1;
        /* X_{k+1} = X_k + 2*Y_k,  Y_{k+1} = X_k + Y_k  */
        lenX[next] = lenX[max_level] + (2 * lenY[max_level]);
        lenY[next] = lenX[max_level] + lenY[max_level];
        max_level = (int)next;
    }
}

/* Symbol (1 or 2) at 0‑based position pos in the expansion after 'level'
   iterations of the morphism, starting from the given symbol. */
static seq_t get_symbol(idx_t pos, int level, int symbol)
{
    if (level == 0)
        return (seq_t)symbol;

    if (symbol == 1) {
        idx_t l1 = lenX[level - 1];
        idx_t l2 = lenY[level - 1];
        if (pos < l1)
            return get_symbol(pos, level - 1, 1);
        pos -= l1;
        if (pos < l2)
            return get_symbol(pos, level - 1, 2);
        pos -= l2;
        return get_symbol(pos, level - 1, 2);
    } else { /* symbol == 2 */
        idx_t l1 = lenX[level - 1];
        idx_t l2 = lenY[level - 1];
        if (pos < l1)
            return get_symbol(pos, level - 1, 1);
        pos -= l1;
        return get_symbol(pos, level - 1, 2);
    }
}

seq_t a(idx_t n)
{
    /* OEIS offset 1: n starts at 1 */
    if (n == 0)   /* defensive; should never happen */
        return 0;
    idx_t pos = n - 1;          /* 0‑based index */
    ensure_lengths(pos + 1);    /* need length at least pos+1 */
    return get_symbol(pos, max_level, 1);
}
