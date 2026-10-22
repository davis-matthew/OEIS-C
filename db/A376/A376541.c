/*
 * A376541  Natural numbers whose iterated squaring modulo 1000 eventually
 *          enters the 4-cycle 201, 401, 801, 601.
 * https://oeis.org/A376541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A376541: natural numbers whose iterated squaring modulo 1000
   eventually enters the 4‑cycle 201, 401, 801, 601.
   This implementation precomputes, once, the residues r (0 ≤ r < 1000)
   that lead to that cycle when repeated squaring modulo 1000 is applied.
   Then a(n) is found by binary searching for the smallest X with
   count_of_eligible_numbers(≤ X) ≥ n.  */
#include <stdbool.h>   /* for bool, true, false */

static bool goodResidue[1000] = {false};
static int  pref[1001]         = {0};
static int  goodCount          = 0;
static bool init_done          = false;

/* memoised value of how many eligible numbers ≤ X */
static inline idx_t count_upto(idx_t X)
{
    if (X == (idx_t)0) return 0;
    idx_t full = X / (idx_t)1000;
    idx_t rem  = X % (idx_t)1000;
    return full * (idx_t)goodCount + (idx_t)pref[(int)rem + 1];
}

/* one‑time initialization of goodResidue, pref, goodCount */
static void init_once(void)
{
    if (init_done) return;
    /* status: -1 unknown, 0 bad, 1 good, 2 visiting */
    int status[1000];
    for (int i = 0; i < 1000; ++i) status[i] = -1;

    /* nodes that belong to the target cycle */
    bool cycleNode[1000] = {false};
    cycleNode[201] = cycleNode[401] = cycleNode[801] = cycleNode[601] = true;

    for (int s = 0; s < 1000; ++s) {
        if (status[s] != -1) continue;

        /* walk the orbit from s until a known state or a repeat */
        int path[1000];
        int p = 0;
        int cur = s;
        while (1) {
            if (status[cur] == 1) {          /* orbit reaches good state */
                for (int i = 0; i < p; ++i) status[path[i]] = 1;
                break;
            }
            if (status[cur] == 0) {          /* orbit reaches bad state */
                for (int i = 0; i < p; ++i) status[path[i]] = 0;
                break;
            }
            if (status[cur] == 2) {          /* cycle detected within this walk */
                /* determine if the cycle contains a target node */
                bool goodCycle = false;
                int node = cur;
                do {
                    if (cycleNode[node]) goodCycle = true;
                    node = (node * node) % 1000;
                } while (node != cur);
                int val = goodCycle ? 1 : 0;
                for (int i = 0; i < p; ++i) status[path[i]] = val;
                break;
            }

            /* mark current node as in progress */
            status[cur] = 2;
            path[p++] = cur;
            cur = (cur * cur) % 1000;
        }
    }

    /* build table of residues that *square* into a good state */
    for (int r0 = 0; r0 < 1000; ++r0) {
        int r = (r0 * r0) % 1000;
        goodResidue[r0] = (status[r] == 1);
        if (goodResidue[r0]) ++goodCount;
    }

    /* prefix sums of goodResidue for fast counting in a block of 1000 */
    pref[0] = 0;
    for (int i = 0; i < 1000; ++i)
        pref[i + 1] = pref[i] + (goodResidue[i] ? 1 : 0);

    init_done = true;
}

/* -------------------------------------------------------------- */
/* Public function required by the problem statement              */
/* -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;  /* not part of the OEIS offset, but safe */

    init_once();

    /* binary search for smallest X with count_upto(X) >= n */
    idx_t low  = (idx_t)1;
    idx_t high = n * (idx_t)20 + (idx_t)1000;   /* generous upper bound */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (count_upto(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }

    return (seq_t)low;
}
