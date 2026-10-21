/*
 * A248863  Table of bit patterns: The bits of A011545(n) -- approximations to
 *          Pi -- placed into row n in the order MSB to LSB.
 * https://oeis.org/A248863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf, less
 * Formula:   a(n) = nth bit of the concatenation of the binary
 *            representations of A011545(k) (the integer formed by the first k
 *            decimal digits of π), listed from the most‑significant to the
 *            least‑significant bit
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/bitlen_seq_t.h"

static const char pi_digits[] =
    "314159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706792148086513282306647093844609550582231725359408128481117450284102701939852110555964462294895493038196442881097566593344612";

static unsigned char *bit_buffer = NULL;   /* stores bits 0/1 */
static size_t bit_capacity   = 0;          /* allocated size of bit_buffer */
static size_t bit_len        = 0;          /* total bits stored */

/* Current value of A011545(k). */
static seq_t pi_val = (seq_t)0;
/* Index of next decimal digit to use from pi_digits. */
static idx_t pi_digit_index = 0;

/* Ensure bit_buffer can hold at least 'need' bits. */
static void ensure_bit_capacity(size_t need)
{
    if (need <= bit_capacity) return;
    size_t new_cap = bit_capacity ? bit_capacity * 2 : 16;
    if (new_cap < need) new_cap = need;
    unsigned char *tmp = (unsigned char *)realloc(bit_buffer, new_cap);
    if (!tmp) abort();
    bit_buffer = tmp;
    bit_capacity = new_cap;
}

/* Append binary bits of 'val' (MSB first) to bit_buffer. */
static void append_bits(seq_t val)
{
    unsigned bitlen = bitlen_seq_t(val);
    for (int i = (int)bitlen - 1; i >= 0; --i) {
        seq_t mask = ((seq_t)1) << i;
        unsigned bit = (val & mask) ? 1 : 0;
        ensure_bit_capacity(bit_len + 1);
        bit_buffer[bit_len++] = (unsigned char)bit;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* safety for illegal index */

    /* Generate bit buffer up to index n. */
    while (bit_len < (size_t)n) {
        if (pi_digit_index >= strlen(pi_digits))
            abort();   /* insufficient digits in pi_digits */
        pi_val = pi_val * (seq_t)10 + (seq_t)(pi_digits[pi_digit_index] - '0');
        ++pi_digit_index;
        append_bits(pi_val);
    }

    return (seq_t)bit_buffer[n - 1];
}
