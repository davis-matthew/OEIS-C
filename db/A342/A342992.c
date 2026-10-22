/*
 * A342992  Smallest k such that k*n contains only prime digits, or 0 if no
 *          such k exists.
 * https://oeis.org/A342992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 2696, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A342992.
   a(n) = smallest k such that n*k consists only of prime digits
   (2,3,5,7).  Return 0 if no such k exists.  Indexed from 1. */
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    int rem;      /* remainder of the current value modulo n */
    seq_t val;    /* current value composed only of prime digits */
} Node;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* not used by OEIS */

    /* Ensure we can cast the size argument. */
    size_t sz = (size_t)n;
    Node *queue = (Node*)malloc(sz * sizeof(Node));
    if (!queue) return (seq_t)0;
    bool *seen = (bool*)calloc(sz, sizeof(bool));
    if (!seen) { free(queue); return (seq_t)0; }

    const seq_t digits[4] = { (seq_t)2, (seq_t)3, (seq_t)5, (seq_t)7 };
    seq_t sn = (seq_t)n;   /* n as seq_t for modulus/division */

    size_t head = 0, tail = 0;

    /* Enqueue all one‑digit prime‑digit numbers. */
    for (int i = 0; i < 4; ++i) {
        seq_t d = digits[i];
        int r = (int)(d % sn);
        if (!seen[r]) {
            seen[r] = true;
            queue[tail++] = (Node){ r, d };
            if (r == 0)
                return d / sn;   /* k = d / n */
        }
    }

    /* BFS over numbers formed from prime digits. */
    while (head < tail) {
        Node cur = queue[head++];
        for (int i = 0; i < 4; ++i) {
            seq_t new_val = cur.val * (seq_t)10 + digits[i];
            int new_rem = (int)(new_val % sn);
            if (!seen[new_rem]) {
                seen[new_rem] = true;
                queue[tail++] = (Node){ new_rem, new_val };
                if (new_rem == 0)
                    return new_val / sn;
            }
        }
    }

    free(queue);
    free(seen);
    return (seq_t)0;   /* no such multiple found */
}
