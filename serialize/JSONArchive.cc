# include <ostream>

# include <elle/format/json/Object.hh>
# include <elle/format/json/Bool.hh>
# include <elle/format/json/Integer.hh>
# include <elle/format/json/Float.hh>
# include <elle/format/json/String.hh>
# include <elle/format/json/Array.hh>
# include <elle/format/json/Dictionary.hh>
# include <elle/format/json/Null.hh>
# include <elle/format/json/Dictionary.hxx>
# include <elle/format/json/Array.hxx>
# include <elle/format/json/Parser.hh>

# include <elle/serialize/JSONArchive.hh>

using namespace elle::serialize;

OutputJSONArchive::OutputJSONArchive(StreamType& stream, json::Object&& obj)
  : BaseClass(stream)
{
  obj.Save(*this);
}

OutputJSONArchive::OutputJSONArchive(StreamType& stream, json::Object const& obj)
  : BaseClass(stream)
{
  obj.Save(*this);
}

void OutputJSONArchive::Save(std::string const& val)
{
  auto& ss = this->stream();
  ss << '"';
  for (auto it = val.cbegin(), end = val.cend(); it != end; ++it)
    {
      switch (*it)
        {
        case '"':
          ss << "\\\""; break;
        case '\\':
          ss << "\\\\"; break;
        case '/':
          ss << "\\/"; break;
        case '\b':
          ss << "\\b"; break;
        case '\f':
          ss << "\\f"; break;
        case '\n':
          ss << "\\n"; break;
        case '\r':
          ss << "\\r"; break;
        case '\t':
          ss << "\\t"; break;
        default:
          ss << *it; break;
        }
    }
  ss << '"';
}
