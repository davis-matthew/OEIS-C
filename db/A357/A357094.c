/*
 * A357094  E.g.f. satisfies A(x)^A(x) = (1 - x * A(x))^(log(1 - x * A(x)) /
 *          2).
 * https://oeis.org/A357094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..⌊n/2⌋} (2k-1)!! * (n-k+1)^(k-1) *
 *            |Stirling1(n,2k)| , with a(0)=1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation uses recurrence for unsigned Stirling numbers of the first kind: */
/*   s(n,m) = s(n-1,m-1) + (n-1)*s(n-1,m) , s(0,0)=1, s(n,0)=0 for n>0.  */
/* The term (2k-1)!! is pre‑computed as the product of odd numbers up to (2k-1).  */
/* All operations are performed with type seq_t, which is an unsigned integer or an
   arbitrary‑precision integer class. All temporary intermediates are of type seq_t
   and no floating point is used. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build; they are used as-is. */

static seq_t* aVals   = NULL;   /* powersets values a(n) */
static size_t aCap    = 0;      /* allocated capacity for aVals */
static idx_t  aLen    = 0;      /* highest index already computed + 1 */

static seq_t* prevS   = NULL;   /* previous row of Stirling numbers */
static size_t sPrevCap = 0;     /* capacity for prevS (equals current n) */

static seq_t* oddFact = NULL;   /* (2k-1)!! values */
static size_t oddCap  = 0;      /* capacity for oddFact */

/* --- small utility: power of a seq_t base to an idx_t exponent --- */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

/* --- ensure buffer capacities are sufficient --- */
static void ensure_a_capacity(idx_t n)
{
    if ((size_t)(n + 1) <= aCap) return;
    size_t newCap = n + 1;
    aVals = (seq_t *)realloc(aVals, newCap * sizeof(seq_t));
    for (size_t i = aCap; i < newCap; ++i) aVals[i] = 0;
    aCap = newCap;
}

static void ensure_prevS_capacity(idx_t n)
{
    if ((size_t)(n + 1) <= sPrevCap) return;
    sPrevCap = n + 1;
    prevS = (seq_t *)realloc(prevS, sPrevCap * sizeof(seq_t));
    for (size_t i = (size_t)n + 1; i < sPrevCap; ++i) prevS[i] = 0;
}

static void ensure_oddFact_capacity(idx_t k)
{
    if ((size_t)(k + 1) <= oddCap) return;
    size_t newCap = k + 1;
    oddFact = (seq_t *)realloc(oddFact, newCap * sizeof(seq_t));
    if (oddCap == 0) oddFact[0] = 1;   /* (2*0-1)!! = 1  */
    for (size_t i = oddCap; i < newCap; ++i) {
        if (i == 0) oddFact[i] = 1;
        else       oddFact[i] = oddFact[i-1] * (2*i - 1);
    }
    oddCap = newCap;
}

/* --- compute all values up to index n --- */
static void compute_up_to(idx_t n)
{
    if (n < aLen) return;

    ensure_a_capacity(n);
    if (aLen == 0) {
        /* initialise a(0) and the first Stirling row */
        aVals[0] = 1;      /* a(0) = 1 */
        aLen = 1;

        ensure_prevS_capacity(0);
        prevS[0] = 1;      /* s(0,0) = 1 */
    }

    for (idx_t cur = aLen; cur <= n; ++cur) {
        size_t curSize = cur + 1;
        seq_t *currS = (seq_t *)calloc(curSize, sizeof(seq_t));

        /* Stirling recurrence s(cur,m) = s(cur-1,m-1) + (cur-1)*s(cur-1,m) */
        for (idx_t m = 1; m <= cur; ++m) {
            seq_t t1 = 0;  /* s(cur-1, m-1) */
            if (m-1 < (idx_t)sPrevCap) t1 = prevS[m-1];

            seq_t t2 = 0;  /* (cur-1)*s(cur-1, m) */
            if (m < (idx_t)sPrevCap) {
                t2 = (seq_t)(cur - 1) * prevS[m];
            }

            currS[m] = t1 + t2;
        }

        /* Ensure odd factorial table is large enough for k up to ⌊cur/2⌋ */
        idx_t kmax = cur >> 1;
        ensure_oddFact_capacity(kmax);

        /* Compute a(cur) from the summation formula */
        seq_t sum = 0;
        for (idx_t k = 1; 2*k <= cur; ++k) {
            seq_t odd = oddFact[k];
            seq_t powv = pow_seq((seq_t)(cur - k + 1), (idx_t)(k - 1));
            seq_t term = odd * powv * currS[2*k];
            sum += term;
        }
        aVals[cur] = sum;

        /* Prepare for the next iteration */
        free(prevS);
        prevS = currS;
        sPrevCap = curSize;
    }

    aLen = n + 1;
}

/* --- public interface ----------------------------------------- */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return aVals[n];
}
