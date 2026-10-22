/*
 * A330015  a(0) = 0; for n > 0, a(n) is the number of distinct digit strings
 *          that occur at least twice (including any overlapping occurrences)
 *          in the concatenation of all previous terms.
 * https://oeis.org/A330015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct digit strings that occur at least
 *            twice (allowing overlap) in the concatenation of a(0) ... a(n‑1)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIGITS   200000          /* estimated maximum length of the concatenated string */
#define MAX_STATES   (2 * MAX_DIGITS + 5)

/* ---------- global buffers ---------- */
static uint8_t digits[MAX_DIGITS];
static size_t  digits_len = 0;

/* SAM (Suffix Automaton) */
static int sam_link[MAX_STATES];
static int sam_len[MAX_STATES];
static int sam_occ[MAX_STATES];
static int sam_next[MAX_STATES][10];
static int sam_sz;
static int sam_last;

/* auxiliary arrays for counting sort */
static int cnt[MAX_DIGITS + 1];
static int order[MAX_STATES];

/* array for the sequence values */
static seq_t *seq_cache = NULL;
static size_t seq_cap  = 0;
static idx_t  seq_computed = 0;

/* ---------- helpers for SAM ---------- */
static void init_sam(void)
{
    sam_sz  = 1;
    sam_last = 0;
    sam_len[0] = 0;
    sam_link[0] = -1;
    sam_occ[0] = 0;
    for (int i = 0; i < 10; ++i) sam_next[0][i] = -1;
}

/* add one character to SAM */
static void sam_extend(int c)
{
    int cur = sam_sz++;
    sam_len[cur] = sam_len[sam_last] + 1;
    sam_occ[cur] = 1;
    for (int i = 0; i < 10; ++i) sam_next[cur][i] = -1;

    int p = sam_last;
    while (p != -1 && sam_next[p][c] == -1) {
        sam_next[p][c] = cur;
        p = sam_link[p];
    }
    if (p == -1) {
        sam_link[cur] = 0;
    } else {
        int q = sam_next[p][c];
        if (sam_len[p] + 1 == sam_len[q]) {
            sam_link[cur] = q;
        } else {
            int clone = sam_sz++;
            sam_len[clone] = sam_len[p] + 1;
            memcpy(sam_next[clone], sam_next[q], sizeof(sam_next[q]));
            sam_link[clone] = sam_link[q];
            sam_occ[clone] = 0;

            while (p != -1 && sam_next[p][c] == q) {
                sam_next[p][c] = clone;
                p = sam_link[p];
            }
            sam_link[q] = clone;
            sam_link[cur] = clone;
        }
    }
    sam_last = cur;
}

/* count repeated substrings in current SAM */
static seq_t compute_repeated_substrings(void)
{
    /* counting sort by length */
    int maxL = 0;
    for (int i = 0; i < sam_sz; ++i) if (sam_len[i] > maxL) maxL = sam_len[i];

    for (int i = 0; i <= maxL; ++i) cnt[i] = 0;
    for (int i = 0; i < sam_sz; ++i) ++cnt[sam_len[i]];
    for (int i = 1; i <= maxL; ++i) cnt[i] += cnt[i - 1];
    for (int i = sam_sz - 1; i >= 0; --i) {
        int l = sam_len[i];
        order[--cnt[l]] = i;
    }

    for (int i = sam_sz - 1; i > 0; --i) {
        int v = order[i];
        int lnk = sam_link[v];
        if (lnk >= 0) sam_occ[lnk] += sam_occ[v];
    }

    seq_t ans = 0;
    for (int i = 1; i < sam_sz; ++i) {
        if (sam_occ[i] >= 2) {
            ans += (seq_t)(sam_len[i] - sam_len[sam_link[i]]);
        }
    }
    return ans;
}

/* append an unsigned integer to the global digits array */
static void append_value(seq_t v)
{
    char buf[65];
    int pos = 0;
    if (v == 0) {
        buf[pos++] = '0';
    } else {
        seq_t tmp = v;
        while (tmp > 0) {
            int d = (int)(tmp % 10);
            buf[pos++] = '0' + d;
            tmp /= 10;
        }
    }
    for (int i = pos - 1; i >= 0; --i) {
        if (digits_len >= MAX_DIGITS) { /* unlikely but guard */
            /* abort silently, the implementation can be extended if needed */
            return;
        }
        digits[digits_len++] = (uint8_t)(buf[i] - '0');
    }
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    /* initialize on first call */
    static int first = 1;
    if (first) {
        first = 0;
        seq_cap = 64;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_cache[0] = (seq_t)0;
        seq_computed = 0;
        digits_len = 0;
        append_value((seq_t)0);  /* a(0) = 0 is already in the digits array */
    }

    /* compute up to requested n */
    while (seq_computed < n) {
        /* build SAM from current concatenation */
        init_sam();
        for (size_t i = 0; i < digits_len; ++i) sam_extend((int)digits[i]);

        seq_t val = compute_repeated_substrings();

        /* grow cache if necessary */
        if (seq_computed + 1 >= seq_cap) {
            seq_cap <<= 1;
            seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
        }
        seq_cache[seq_computed + 1] = val;
        ++seq_computed;

        append_value(val);
    }

    return seq_cache[n];
}
