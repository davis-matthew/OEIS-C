/*
 * A384191  Unique increasing sequence (a(n)) whose complement, (b(n)),
 *          satisfies b(n) = a(a(n))+3.
 * https://oeis.org/A384191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A384191: a(n) is the increasing sequence whose complement b(n)
    satisfies b(n) = a(a(n)) + 3.
    The algorithm builds both sequences incrementally,
    keeping a bitmap of used integers and a table that records
    pending indices awaiting the value of a(a(n)).
*/
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*  The types seq_t and idx_t are defined by the build environment.
    Ensure the external declaration so the compiler does not warn.   */
extern seq_t a(idx_t n);

/*--- Dynamic arrays -------------------------------------------------------*/
static seq_t   *A      = NULL;   /* a[1..Alen] (1‑based) */
static size_t  Acap   = 0;       /* allocated capacity (number of slots)   */
static idx_t   Alen   = 0;       /* number of a values known */

static unsigned char *USED   = NULL;   /* bitmap of values already used   */
static size_t        UsedCap = 0;      /* size of USED in bytes           */

static idx_t *PENDING = NULL;         /* PENDING[v] = index k s.t. a(k)=v, 0 if none */
static size_t PendingCap = 0;          /* size of PENDING array */


/*--- Reallocation helpers -----------------------------------------------*/
static void ensure_A_capacity(size_t need)             /* need: index+1   */
{
    if (need <= Acap) return;
    size_t newcap = Acap ? Acap * 2 : 16;
    while (newcap < need) newcap <<= 1;
    seq_t *newA = (seq_t *)realloc(A, newcap * sizeof(seq_t));
    if (!newA) abort();
    A = newA;
    Acap = newcap;
}

static void ensure_USED_capacity(idx_t v)
{
    if ((size_t)v < UsedCap) return;
    size_t newcap = UsedCap ? UsedCap << 1 : 16;
    while ((size_t)newcap <= (size_t)v) newcap <<= 1;
    unsigned char *newused = (unsigned char *)realloc(USED, newcap);
    if (!newused) abort();
    memset(newused + UsedCap, 0, newcap - UsedCap);
    USED = newused;
    UsedCap = newcap;
}

static void ensure_PENDING_capacity(idx_t v)
{
    if ((size_t)v < PendingCap) return;
    size_t newcap = PendingCap ? PendingCap << 1 : 16;
    while ((size_t)newcap <= (size_t)v) newcap <<= 1;
    idx_t *newp = (idx_t *)realloc(PENDING, newcap * sizeof(idx_t));
    if (!newp) abort();
    /* initialise new slots to 0 */
    memset(newp + PendingCap, 0, (newcap - PendingCap) * sizeof(idx_t));
    PENDING = newp;
    PendingCap = newcap;
}

/*--- Process encounters of the form a(a(k)) -----------------------------*/
static void process_pending(idx_t idx)
/* idx = v, the value for which all a(k)=v are ready to produce b(k). */
{
    if ((size_t)idx >= PendingCap) return;
    idx_t k = PENDING[idx];
    if (k == 0) return;                /* no pending index */
    seq_t bval = A[idx] + 3;           /* a(idx) is already known, so compute b(k) */
    idx_t bidx = (idx_t)bval;
    ensure_USED_capacity(bidx);
    USED[(size_t)bidx] = 1;            /* mark the complementary number used */
    PENDING[idx] = 0;                   /* clear after handling */
}

/*--- Main sequence function ---------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n=0; return 0 quietly */

    while (Alen < n) {
        /* Find the next unused integer > a(Alen) (or 0 if none). */
        seq_t last_val = (Alen == 0) ? 0 : A[Alen];
        idx_t cand = (idx_t)last_val + 1;
        while (true) {
            ensure_USED_capacity(cand);
            if (!USED[(size_t)cand]) break;
            ++cand;
        }

        /* Store the new a value. */
        ++Alen;
        ensure_A_capacity(Alen + 1);            /* indices are 1-based */
        A[Alen] = cand;
        ensure_USED_capacity(cand);
        USED[(size_t)cand] = 1;                 /* mark as used */

        /* Record that a(Alen)=cand, so when a(cand) is known
           we can compute b(Alen) = a(cand)+3. */
        ensure_PENDING_capacity(cand);
        PENDING[cand] = Alen;

        /* If the value equals its own index (identity part of the
           sequence) then the pending entry targets the current index
           and can be processed immediately. */
        process_pending(Alen);
    }

    return A[(size_t)n];
}
