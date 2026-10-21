/*
 * A261871  Numbers of the form (2*j-1)*(2^k-1); j>=1, k>=2.
 * https://oeis.org/A261871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th smallest positive integer of the form
 *            (2*j-1)*(2^k-1) with j≥1, k≥2; sorted ascendingly and indexed
 *            from 1
 * Range:     u8 n <= 29, u16 n <= 8227, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, this case is unused */

    /* Approximate an upper bound for the n-th term:  a(n) < 5n   (OEIS). */
    seq_t limit = (seq_t)5 * n + 10;   /* small safety margin */

    /* Heap node holding current term, the odd multiplier and the Mersenne base. */
    typedef struct {
        seq_t val;   /* current term (M * odd) */
        seq_t odd;   /* odd multiplier (1,3,5,...) */
        seq_t M;     /* Mersenne number 2^k - 1 */
    } node_t;

    node_t heap[64];  /* enough for all k with M <= limit (k ≤ 60 for 64-bit shifts) */
    int hsz = 0;

    /* Build initial entries for all Mersenne numbers up to the bound. */
    seq_t M = 3;  /* 2^2 - 1 */
    while (hsz < 64 && M <= limit) {
        node_t node;
        node.M   = M;
        node.odd = 1;            /* j = 1  -> odd multiplier 1 */
        node.val = M;            /* M * 1 */

        /* Insert into binary min‑heap. */
        int i = hsz;
        heap[i] = node;
        hsz++;

        while (i > 0) {
            int p = (i - 1) >> 1;
            if (heap[p].val <= heap[i].val) break;
            node_t tmp = heap[p];
            heap[p] = heap[i];
            heap[i] = tmp;
            i = p;
        }

        /* Next Mersenne number:  M = (2^k - 1) → (2^ (k+1) - 1) = 2*M + 1 */
        M = M * 2 + 1;
    }

    seq_t last = 0;   /* last output term to filter duplicates */
    int count = 0;    /* how many unique terms found so far */

    while (1) {
        /* Extract the smallest term from the heap. */
        node_t node = heap[0];
        hsz--;
        heap[0] = heap[hsz];

        /* Restore heap property. */
        int i = 0;
        while (1) {
            int l = (i << 1) + 1;
            int r = l + 1;
            int smallest = i;
            if (l < hsz && heap[l].val < heap[smallest].val) smallest = l;
            if (r < hsz && heap[r].val < heap[smallest].val) smallest = r;
            if (smallest == i) break;
            node_t tmp = heap[i];
            heap[i] = heap[smallest];
            heap[smallest] = tmp;
            i = smallest;
        }

        /* Emit unique terms in ascending order. */
        if (node.val != last) {
            count++;
            if (count == (int)n) return node.val;
            last = node.val;
        }

        /* Prepare the next odd multiple for the same Mersenne number. */
        node.odd += 2;
        node.val = node.M * node.odd;

        /* Insert back into the heap. */
        int j = hsz;
        heap[j] = node;
        hsz++;

        while (j > 0) {
            int p = (j - 1) >> 1;
            if (heap[p].val <= heap[j].val) break;
            node_t tmp = heap[p];
            heap[p] = heap[j];
            heap[j] = tmp;
            j = p;
        }
    }
}
