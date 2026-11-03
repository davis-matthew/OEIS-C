/*
 * A352245  a(0) = 1; for n >= 1, a(n) = the decimal value of the binary
 *          number of the index of where n first appears in the concatenation
 *          of all previous binary terms. If the binary value of n has not
 *          previously appeared then a(n) = 0.
 * https://oeis.org/A352245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Validated: partial, pass, 93 terms through n = 92 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A352245: a(0)=1, for n>=1
   a(n) = the decimal value of the binary number of the index where
   the binary representation of n first appears in the concatenation
   of all previous binary values.  If it never appears, a(n)=0.  */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static size_t bin_str_from_uint64(idx_t v, char *buf)
/* fill buf with the binary representation of v (most‑significant bit first)
   and return its length */
{
    if (v == (idx_t)0) { buf[0] = '0'; return 1; }
    size_t len = 0;
    idx_t tmp = v;
    while (tmp > 0) { buf[len++] = (tmp & (idx_t)1) ? '1' : '0'; tmp >>= 1; }
    /* reverse to get MSB first */
    for (size_t i = 0; i < len / 2; ++i) {
        char c = buf[i]; buf[i] = buf[len - 1 - i]; buf[len - 1 - i] = c;
    }
    return len;
}

static long long find_first_occurrence(const char *txt, size_t txtlen,
                                       const char *pat, size_t patlen)
/* return 0‑based index of first occurrence of pat in txt.
   If not found, return -1.  */
{
    if (patlen == 0) return 0;
    if (txtlen < patlen) return -1;
    for (size_t i = 0; i + patlen <= txtlen; ++i) {
        size_t j;
        for (j = 0; j < patlen; ++j)
            if (txt[i + j] != pat[j]) break;
        if (j == patlen) return (long long)i;
    }
    return -1;
}

/* ---------  helper functions ----------------------------------- */

static unsigned bitlen_seq_t(seq_t x)
/* number of bits needed to represent x (0 gives 0) */
{
    unsigned l = 0;
    while (x) { l++; x >>= 1; }
    return l;
}

/* ---------  global state --------------------------------------- */

static seq_t *a_cache = NULL;   /* sequence values */
static size_t a_len = 0, a_cap = 0;

static char *bin_str = NULL;      /* concatenated binary string */
static size_t bin_len = 0, bin_cap = 0;

static void init_global_state(void)
{
    if (a_cache != NULL) return;

    a_cap = 128;
    a_cache = mem_new(seq_t, a_cap);
    a_len = 0;

    bin_cap = 1024;
    bin_str = (char *)malloc(bin_cap);
    bin_len = 0;

    a_cache[a_len++] = (seq_t)1;   /* a(0) = 1 */
    bin_str[bin_len++] = '1';      /* binary of 1 */
}

static void append_bin_of_seq(seq_t v)
/* append binary representation of v to the global bin_str */
{
    unsigned bitlen = bitlen_seq_t(v);
    if (bitlen == 0) bitlen = 1;   /* represent 0 as a single '0' */

    /* ensure capacity */
    size_t need = bin_len + bitlen;
    if (need > bin_cap) {
        size_t newcap = bin_cap ? bin_cap * 2 : 256;
        while (newcap < need) newcap *= 2;
        char *tmp = (char *)realloc(bin_str, newcap);
        if (!tmp) return;          /* allocation failed – give up quietly */
        bin_str = tmp;
        bin_cap = newcap;
    }

    /* write bits from MSB to LSB */
    for (unsigned i = 0; i < bitlen; ++i) {
        size_t shift = bitlen - 1 - i;
        bin_str[bin_len++] = (((v >> shift) & (seq_t)1) ? '1' : '0');
    }
}

/* ---------  public function ------------------------------------- */

seq_t a(idx_t n)
{
    init_global_state();

    /* build the sequence up to n if needed */
    if ((size_t)n >= a_len) {
        /* enlarge cache if necessary */
        if ((size_t)n + 1 > a_cap) {
            size_t newcap = a_cap ? a_cap * 2 : 256;
            while (newcap < (size_t)n + 1) newcap *= 2;
            seq_t *tmp = mem_resize(seq_t, a_cache, newcap);
            if (!tmp) return (seq_t)0;     /* allocation failure */
            a_cache = tmp;
            a_cap = newcap;
        }

        /* compute terms from current length to n (inclusive) */
        for (size_t i = a_len; i <= (size_t)n; ++i) {
            char pat[65];
            size_t patlen = bin_str_from_uint64((idx_t)i, pat);

            long long pos = find_first_occurrence(bin_str, bin_len, pat, patlen);

            seq_t val = (pos < 0) ? (seq_t)0 : (seq_t)(pos + 1);
            a_cache[i] = val;

            append_bin_of_seq(val);
        }
        a_len = (size_t)n + 1;
    }

    return a_cache[n];
}
