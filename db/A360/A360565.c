/*
 * A360565  Denominators of breadth-first numerator-denominator-incrementing
 *          enumeration of rationals in (0,1).
 * https://oeis.org/A360565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn, tabf, look
 * Formula:   a(n) = denominator of the n‑th node in the breadth‑first *
 *            enumeration of rationals in (0,1) defined by * * * start with
 *            1/2 * * for each node p/q add children p/(q+1) and (p+1)/q * (in
 *            that order) if the child is < 1 and not seen before
 * Range:     u8 n <= 8098, u16 n <= 9675, u32 n <= 9675, u64 n <= 9675,
 *            u128 n <= 9675, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

#define MAX_NODES 200000   /* sufficient for n up to about 10 000 */

static seq_t node_num[MAX_NODES];
static seq_t node_den[MAX_NODES];
static idx_t head  = 0;           /* index of node to process next     */
static idx_t tail  = 0;           /* number of nodes stored             */
static idx_t built = 0;           /* number of terms already built      */

/* initialise the queue with the root 1/2 */
static void init(void)
{
    if (tail != 0) return;
    node_num[0] = 1;
    node_den[0] = 2;
    tail = 1;
    head = 0;
    built = 0;
}

/* generate terms until term n is available */
static void ensure_size(idx_t n)
{
    if (built >= n) return;          /* already built enough            */
    if (tail == 0) init();           /* first call                      */

    while (built < n) {
        seq_t p = node_num[head];
        seq_t q = node_den[head];

        /* child 1: p/(q+1) */
        seq_t a1 = p;
        seq_t b1 = q + 1;
        seq_t g1 = gcd(a1, b1);
        a1 /= g1;   b1 /= g1;
        bool dup = false;
        for (idx_t i = 0; i < tail; ++i) {
            if (node_num[i] == a1 && node_den[i] == b1) { dup = true; break; }
        }
        if (!dup) {
            node_num[tail] = a1;
            node_den[tail] = b1;
            ++tail;
        }

        /* child 2: (p+1)/q  (only if < 1) */
        seq_t a2 = p + 1;
        seq_t b2 = q;
        if (a2 < b2) {
            seq_t g2 = gcd(a2, b2);
            a2 /= g2;   b2 /= g2;
            bool dup2 = false;
            for (idx_t i = 0; i < tail; ++i) {
                if (node_num[i] == a2 && node_den[i] == b2) { dup2 = true; break; }
            }
            if (!dup2) {
                node_num[tail] = a2;
                node_den[tail] = b2;
                ++tail;
            }
        }

        ++head;
        ++built;      /* one more term produced */
    }
}

/* required OEIS function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* undefined for 0, but guard anyway */
    ensure_size(n);
    return node_den[n - 1];
}
