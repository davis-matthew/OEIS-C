/*
 * A246146  Limiting block extension of A010060 (Thue-Morse sequence) with
 *          first term as initial block.
 * https://oeis.org/A246146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   bit_parity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: A246146 – limiting block extension of the Thue–Morse sequence
 * starting with block B(0,0) = (s(0)).
 *
 * Algorithm:
 *   Let m(i) be the starting index of the i‑th block B'(i) of length i
 *   in the Thue–Morse sequence S.  B'(0) = (s(0)), m(0)=0.
 *   For i ≥ 1, find the smallest position t > m(i‑1) such that the
 *   block S[t … t+i‑1] has the same first i‑1 symbols as B'(i‑1).
 *   Then m(i) = t and a(i) = S[t+i] (the last symbol of B'(i)).
 *   The Thue–Morse symbol S[n] is the parity of the popcount of n.
 *
 *   This implementation caches all previously computed m(i) and a(i)
 *   values, so repeated calls with arbitrary n are O(1) after the
 *   first computation up to the largest requested n.  The search of
 *   the next occurrence uses the Knuth–Morris–Pratt (KMP) algorithm.
 *
 *   Complexity: O(n²) worst case, acceptable for n ≤ 10 000.
 *   No external libraries are required beyond C/C++ standard headers.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/bit_parity.h"

/* cached data ---------------------------------------------------------- */
static idx_t *m_vals  = NULL;   /* start indices m(i) */
static seq_t *a_vals  = NULL;   /* returned values a(i) */
static size_t m_len   = 0;      /* number of cached entries    */
static size_t m_cap   = 0;      /* allocated capacity          */

static seq_t *pattern = NULL;   /* temporary pattern buffer    */
static int   *pi      = NULL;   /* KMP prefix-function         */
static size_t pat_cap = 0;      /* capacity of pattern/pi      */

/* ensure sufficient capacity for index/value tables ------------------- */
static void ensure_index_capacity(size_t need)
{
    if (m_cap < need) {
        size_t newcap = (m_cap > 0) ? m_cap << 1 : need;
        if (newcap < need) newcap = need;

        idx_t  *tmpM = (idx_t *)realloc(m_vals, newcap * sizeof(idx_t));
        seq_t *tmpA = (seq_t *)realloc(a_vals, newcap * sizeof(seq_t));
        if (!tmpM || !tmpA) abort();          /* allocation failure */
        m_vals = tmpM;
        a_vals = tmpA;
        m_cap  = newcap;
    }
}

/* ensure sufficient capacity for pattern and pi arrays ---------------- */
static void ensure_pattern_capacity(size_t need)
{
    if (pat_cap < need) {
        size_t newcap = need;
        seq_t *tmpP = (seq_t *)realloc(pattern, newcap * sizeof(seq_t));
        int   *tmpI = (int   *)realloc(pi,      newcap * sizeof(int));
        if (!tmpP || !tmpI) abort();
        pattern = tmpP;
        pi      = tmpI;
        pat_cap = newcap;
    }
}

/* --------------------------------------------------------------------- */
/*  seq_t a(idx_t n)                                                   */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n + 1 > m_cap)              /* extend storage if needed */
        ensure_index_capacity(n + 1);

    /* compute sequence up to index n if not already done */
    for (idx_t i = m_len; i <= n; ++i) {
        if (i == 0) {                     /* initial term */
            m_vals[0] = 0;
            a_vals[0] = bit_parity(0);    /* Thue–Morse of 0 = 0 */
        } else {
            idx_t prev_start = m_vals[i - 1];
            uint64_t len = i;             /* length of the previous block */

            /* build pattern = S[prev_start … prev_start+len-1] */
            ensure_pattern_capacity((size_t)len);
            for (uint64_t j = 0; j < len; ++j)
                pattern[j] = bit_parity(prev_start + j);

            /* build KMP prefix function for the pattern */
            pi[0] = 0;
            int k = 0;
            for (size_t j = 1; j < (size_t)len; ++j) {
                while (k > 0 && pattern[j] != pattern[k])
                    k = pi[k - 1];
                if (pattern[j] == pattern[k]) ++k;
                pi[j] = k;
            }

            /* KMP search for the next occurrence after prev_start */
            uint64_t t = prev_start + 1;
            k = 0;
            uint64_t new_start = 0;          /* will be overwritten once found */
            while (1) {
                seq_t ch = bit_parity(t);
                while (k > 0 && ch != pattern[k])
                    k = pi[k - 1];
                if (ch == pattern[k]) ++k;
                if (k == (int)len) {          /* match found */
                    new_start = t - len + 1;  /* starting position of match */
                    break;
                }
                ++t;
            }

            m_vals[i] = new_start;
            a_vals[i] = bit_parity(new_start + len);   /* last symbol of block */
        }
    }

    m_len = n + 1;   /* all entries up to n are now cached */
    return a_vals[n];
}
